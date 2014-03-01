//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtRecord.cpp *#*#*#*#*#*#*#*# (C) 2000-2010 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <stdio.h>
#include "DTAPI.h"
#include "DtRecord.h"
#include "DtSdiFileFmt.h"

#ifdef WIN32
	#include <conio.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtRecord Version -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTRECORD_VERSION_MAJOR		3
#define DTRECORD_VERSION_MINOR		7
#define DTRECORD_VERSION_BUGFIX		0

// Command line option flags
const char c_DvcTypeCmdLineFlag[]			= "t";
const char c_DvcNumCmdLineFlag[]			= "n";
const char c_PortCmdLineFlag[]				= "i";

const char c_RxModeCmdLineFlag[]			= "m";
const char c_RxModeHuffmanCmdLineFlag[]		= "c";
const char c_MaxSizeCmdLineFlag[]			= "x";

const char c_ModTypeCmdLineFlag[]			= "mt";
const char c_CarrierFreqCmdLineFlag[]		= "mf";
const char c_QamJ83AnnexCmdLineFlag[]		= "ma";
const char c_ModBandwidthCmdLineFlag[]		= "mb";

const char c_IpAddressCmdLineFlag[]			= "ipa";
const char c_IpProtocolCmdLineFlag[]		= "ipp";

const char c_LndVoltageCmdLineFlag[]		= "lnbv";
const char c_LndToneCmdLineFlag[]			= "lnbt";

//const char c_DiseqcCommandCmdLineFlag[]		= "diseqc";
const char c_DiseqcPortGroupCmdLineFlag[]	= "diseqcpg";

const char c_PolarityControlCmdLineFlag[]	= "pc";

const char c_SilentModeCmdLineFlag[]		= "s";
const char c_HelpCmdLineFlag[]				= "?";

const int c_BufSize = 2*1024*1024;		// Data transfer buffer size

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Error messages -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Command line errors
const char c_CleUnknownOption[]			= "Unknown command-line option (-%s) encountered";
const char c_CleMissingArgument[]		= "Missing argument for command-line option: -%s";
const char c_CleNoArgumentExpected[]	= "-%s command-line option does not have a argument";
const char c_CleInvalidArgument[]		= "Invalid argument for command line option: -%s";
const char c_CleNoRecFile[]				= "No record file specified";
const char c_CleTxRateNotDefined[]		= "Transport-Stream rate not specified";

// General errors
const char c_ErrDtapiHwScanFailed[]		= "DtapiHwFuncScan failed (ERROR: %s)";
const char c_ErrNoInputFound[]			= "No input device in the system";
const char c_ErrNoDta1xxFound[]			= "No DTA-%d with input port %d found";
const char c_ErrNoDtu2xxFound[]			= "No DTU-%d with input port %d found";
const char c_ErrCouldNotFindInputN[]	= "Could not find input board (#%d, port=%d) in the system";
const char c_ErrCouldNotFindDta1xxN[]	= "Could not find DTA-%d (#%d, port=%d) in the system";
const char c_ErrCouldNotFindDtu2xxN[]	= "Could not find DTU-%d (#%d, port=%d) in the system";
const char c_ErrFailToAttachToDta1xx[]	= "Failed to attach to the DTA-%d on Bus: %d and Slot: %d";
const char c_ErrFailToAttachToDtu2xx[]	= "Failed to attach to the DTU-%d";
const char c_ErrFailToAttachToChan[]	= "Can't attach to the channel (ERROR: %s)";
const char c_ErrFailToOpenFile[]		= "Can't open '%s' for writing";
const char c_ErrWriteFile[]				= "File write error";
const char c_ErrFailSetRxControl[]		= "SetRxControl failed (ERROR: %s)";
const char c_ErrFailSetRxMode[]			= "SetRxMode failed (ERROR: %s)";
const char c_ErrFailSetPolarCtrl[]		= "SetPolarityControl failed (ERROR: %s)";
const char c_ErrFailSetDemodControl[]	= "SetModControl failed (ERROR: %s)";
const char c_ErrFailSetRfControl[]		= "SetRfControl failed (ERROR: %s)";
const char c_ErrFailSetIpPars[]			= "SetIpPars failed (ERROR: %s)";
const char c_ErrFailRead[]				= "Read failed (ERROR: %s)";
const char c_ErrFailWriteDtSdiFileHdr[]	= "Failed to write DTSDI file header";

const char c_ErrDriverInCompPci[] =
	"The current Dta1xx driver (V%d.%d.%d %d) is not compatible with this\n" \
	"version of DtRecord.\n" \
	"Please install the latest version of the Dta1xx driver.";

const char c_ErrDriverInCompUsb[] =
	"The current Dtu2xx driver (V%d.%d.%d %d) is not compatible with this\n" \
	"version of DtRecord.\n" \
	"Please install the latest version of the Dtu2xx driver.";

// Warning messages
const char c_WarnHwFifoOverflow[] = 
	"WARNING: hardware-FIFO overflow detected => possible corruption of record file";


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

	m_DvcType = -1;
	m_DvcNum = 1;
	m_Port = 1;

	m_RxMode = -1;
	m_RxModeHuffman = false;
	m_MaxSize = -1;

	m_ModType = -1;
	m_Bandwidth = -1;
	m_CarrierFreq = -1.0;
	m_QamJ83Annex = DTAPI_MOD_J83_B;

	m_LnbVoltage = -1;
	m_LnbToneEnable = false;
	m_DiseqcPortGroup = -1;

	memset(&m_IpPars, 0, sizeof(m_IpPars));
	// Set default IP parameters
	m_IpPars.m_Port = 5678;
	m_IpPars.m_Protocol = DTAPI_PROTO_AUTO;
	
	m_Polarity = -1;

	m_SilentMode = false;
	m_ShowHelp = false;

	m_RecDtSdiFile = false;
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
		throw Exc(c_CleNoRecFile);

	if ( m_RxMode!=-1 && 0!=(m_RxMode & DTAPI_RXMODE_SDI ) )
		m_RecDtSdiFile = true;
	else
		m_RecDtSdiFile = false;

	// Check for huffman compression
	if ( m_RecDtSdiFile && m_RxModeHuffman )
		m_RxMode |= DTAPI_RXMODE_SDI_HUFFMAN;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Set dynamic option default -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Set default settings for parameters which are not defined on the command line

	// RxMode defined?
	if ( m_RxMode == -1  )
		m_RxMode = DTAPI_RXMODE_ST188;

	// Carrier defined?
	if ( m_CarrierFreq == -1.0 )
	{
		//if ( m_ModType==DTAPI_MOD_QPSK || m_ModType==DTAPI_MOD_BPSK )
		//	m_CarrierFreq = 1915.0;		// L-Band
		//else /* ISDBT, OFDM, QAM */
			m_CarrierFreq = 578.0;		// UHF-band
	}
	
	if ( m_Polarity == -1)
	{
		if ( m_RxMode == DTAPI_RXMODE_STRAW )
			m_Polarity = DTAPI_POLARITY_NORMAL;
		else
			m_Polarity = DTAPI_POLARITY_AUTO;
	}

	if ( m_Bandwidth == -1 )
	{
		if ( m_ModType == DTAPI_MOD_DVBT )
			m_Bandwidth = DTAPI_MOD_DVBT_8MHZ;
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

	if ( 0==strcmp(pParam, c_DvcTypeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_DvcNumCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_PortCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_RxModeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_RxModeHuffmanCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_ModTypeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_CarrierFreqCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_QamJ83AnnexCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_ModBandwidthCmdLineFlag) )
		m_LastFlagReqsArg = true;

	else if ( 0==strcmp(pParam, c_LndVoltageCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_LndToneCmdLineFlag) )
	{
		m_LastFlagReqsArg = false;
		m_LnbToneEnable = true;
	}
	else if ( 0==strcmp(pParam, c_DiseqcPortGroupCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_MaxSizeCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_IpAddressCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_IpProtocolCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_PolarityControlCmdLineFlag) )
		m_LastFlagReqsArg = true;
	else if ( 0==strcmp(pParam, c_SilentModeCmdLineFlag))
	{
		m_SilentMode = true;
		m_LastFlagReqsArg = false;
	}
	else if ( 0==strcmp(pParam, c_HelpCmdLineFlag))
	{
		m_ShowHelp = true;
		m_LastFlagReqsArg = false;
	}
	else
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


	if ( 0==strcmp(m_pLastFlag, c_DvcTypeCmdLineFlag) )
	{
		m_DvcType = atoi(pParam);
		if ( m_DvcType < 100 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DvcNumCmdLineFlag) )
	{
		m_DvcNum = atoi(pParam);
		if ( m_DvcNum < 1 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_PortCmdLineFlag) )
	{
		m_Port = atoi(pParam);
		if ( m_Port < 1 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_RxModeCmdLineFlag) )
	{
		// Which rx-mode
		if ( 0 == strcmp(pParam, "ST188")  )
			m_RxMode = DTAPI_RXMODE_ST188;
		else if ( 0 == strcmp(pParam, "ST204")  )
			m_RxMode = DTAPI_RXMODE_ST204;
		else if ( 0 == strcmp(pParam, "STRAW")  )
			m_RxMode = DTAPI_RXMODE_STRAW;
		else if ( 0 == strcmp(pParam, "STTRP")  )
			m_RxMode = DTAPI_RXMODE_STTRP | DTAPI_RX_TIMESTAMP32;
        else if ( 0 == strcmp(pParam, "ST188T")  )
            m_RxMode = DTAPI_RXMODE_ST188 | DTAPI_RX_TIMESTAMP32;
        else if ( 0 == strcmp(pParam, "ST204T")  )
            m_RxMode = DTAPI_RXMODE_ST204 | DTAPI_RX_TIMESTAMP32;
		else if ( 0 == strcmp(pParam, "SDI8B")  )
			m_RxMode = DTAPI_RXMODE_SDI_FULL;
		else if ( 0 == strcmp(pParam, "SDI10B")  )
			m_RxMode = DTAPI_RXMODE_SDI_FULL | DTAPI_RXMODE_SDI_10B;
		else if ( 0 == strcmp(pParam, "SDI8B_ACTVID")  )
			m_RxMode = DTAPI_RXMODE_SDI_ACTVID;
		else if ( 0 == strcmp(pParam, "SDI10B_ACTVID")  )
			m_RxMode = DTAPI_RXMODE_SDI_ACTVID | DTAPI_RXMODE_SDI_10B;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_RxModeHuffmanCmdLineFlag) )
	{
		if ( 0 == strcmp(pParam, "ON")  )
			m_RxModeHuffman = true;
		else if ( 0 == strcmp(pParam, "OFF")  )
			m_RxModeHuffman = false;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_ModTypeCmdLineFlag) )
	{
		// Which modulation mode
		if ( 0 == strcmp(pParam, "QAM64")  )
			m_ModType = DTAPI_MOD_QAM64;
		else if ( 0 == strcmp(pParam, "QAM256")  )
			m_ModType = DTAPI_MOD_QAM256;
		else if ( 0 == strcmp(pParam, "VSB8")  )
			m_ModType = DTAPI_MOD_ATSC;
		else if ( 0 == strcmp(pParam, "ATSC")  )
			m_ModType = DTAPI_MOD_ATSC;
		else if ( 0 == strcmp(pParam, "DVBT") )
			m_ModType = DTAPI_MOD_DVBT;
		else if ( 0 == strcmp(pParam, "DVBS") )
			m_ModType = DTAPI_MOD_DVBS_QPSK; // or DTAPI_MOD_DVBS_BPSK
		else if ( 0 == strcmp(pParam, "DVBS2") )
			m_ModType = DTAPI_MOD_DVBS2_QPSK; // or DTAPI_MOD_DVBS2_8PSK
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	
	}
	else if ( 0==strcmp(m_pLastFlag, c_CarrierFreqCmdLineFlag) )
	{
        m_CarrierFreq = atof(pParam);
		if ( m_CarrierFreq < 0.0 )
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_QamJ83AnnexCmdLineFlag) )
	{
		if ( 0 == strcmp(pParam, "A")  )
			m_QamJ83Annex = DTAPI_MOD_J83_A;
		else if ( 0 == strcmp(pParam, "B") )
			m_QamJ83Annex = DTAPI_MOD_J83_B;
		else if ( 0 == strcmp(pParam, "C") )
			m_QamJ83Annex = DTAPI_MOD_J83_C;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_ModBandwidthCmdLineFlag) )
	{
//		// Modulation type should be defined first
//		if ( m_ModType != DTAPI_MOD_DVBT )
//            throw Exc(c_CleModTypeNotDefined, m_pLastFlag);

		if ( 0 == strcmp(pParam, "6") ) {
			m_Bandwidth = DTAPI_MOD_DVBT_6MHZ;
		}
		else if ( 0 == strcmp(pParam, "7") ) {
			m_Bandwidth = DTAPI_MOD_DVBT_7MHZ;
		}
		else if ( 0 == strcmp(pParam, "8") ) {
			m_Bandwidth = DTAPI_MOD_DVBT_8MHZ;
		}
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_LndVoltageCmdLineFlag) )
	{
		m_LnbVoltage = atoi(pParam);
		if ( m_LnbVoltage == 13 )
			m_LnbVoltage = DTAPI_LNB_13V;
		else if ( m_LnbVoltage == 14 )
			m_LnbVoltage = DTAPI_LNB_14V;
		else if ( m_LnbVoltage == 18 )
			m_LnbVoltage = DTAPI_LNB_18V;
		else if ( m_LnbVoltage == 19 )
			m_LnbVoltage = DTAPI_LNB_19V;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_DiseqcPortGroupCmdLineFlag) )
	{
		m_DiseqcPortGroup = (int)strtol(pParam, NULL, 16);
		if (m_DiseqcPortGroup<0xF0 || m_DiseqcPortGroup>0xFF)
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_MaxSizeCmdLineFlag) )
	{
		m_MaxSize = atoi(pParam);
		if ( m_MaxSize < 0 )
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
		if (0 == strcmp(pParam, "AUTO") )
			m_IpPars.m_Protocol = DTAPI_PROTO_AUTO;
		if ( 0 == strcmp(pParam, "UDP")  )
			m_IpPars.m_Protocol = DTAPI_PROTO_UDP;			
		else if ( 0 == strcmp(pParam, "RTP")  )
			m_IpPars.m_Protocol = DTAPI_PROTO_RTP;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else if ( 0==strcmp(m_pLastFlag, c_PolarityControlCmdLineFlag) )
	{
		if (0 == strcmp(pParam, "NORMAL") )
			m_Polarity = DTAPI_POLARITY_NORMAL;
		else if (0 == strcmp(pParam, "INVERT") )
			m_Polarity = DTAPI_POLARITY_INVERT;
		else if (0 == strcmp(pParam, "AUTO") )
			m_Polarity = DTAPI_POLARITY_AUTO;
		else
			throw Exc(c_CleInvalidArgument, m_pLastFlag);
	}
	else
		throw Exc(c_CleUnknownOption, m_pLastFlag);

	// Clear required argument flag
	m_LastFlagReqsArg = false;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::RxMode2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::RxMode2Str() const
{
	static char sz[256] = "?";
	if ( m_RxMode & DTAPI_RXMODE_SDI )
	{
		if ( (m_RxMode & (DTAPI_RXMODE_SDI | 0x7)) == DTAPI_RXMODE_SDI_FULL )
		{
			if ( 0!=(m_RxMode & DTAPI_RXMODE_SDI_10B ) )
				strcpy(sz, "SDI10B");
			else
				strcpy(sz, "SDI8B");
		}
		else if ( (m_RxMode & (DTAPI_RXMODE_SDI | 0x7)) == DTAPI_RXMODE_SDI_ACTVID )
		{
			if ( 0!=(m_RxMode & DTAPI_RXMODE_SDI_10B ) )
				strcpy(sz, "SDI10B_ACTVID");
			else
				strcpy(sz, "SDI8B_ACTVID");
		} else
			return "?";

		// Check for Huffmann compression
		if ( 0!=(m_RxMode & DTAPI_RXMODE_SDI_HUFFMAN) )
			strcat(sz, " (Huffman compressed)");

		return sz;
	}
	else
	{
		switch( m_RxMode&0x7 )
		{
        case DTAPI_RXMODE_ST188:	
            return (m_RxMode & DTAPI_RX_TIMESTAMP32)==0 ? "ST188" : "ST188T";
		case DTAPI_RXMODE_ST204:
            return (m_RxMode & DTAPI_RX_TIMESTAMP32)==0 ? "ST204" : "ST204T";
		case DTAPI_RXMODE_STRAW:	return "STRAW";
		case DTAPI_RXMODE_STTRP:	return "STTRP";
		default:					return "?";
		}
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::ModType2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::ModType2Str() const
{
	switch( m_ModType )
	{
	case DTAPI_MOD_QAM64:		return "QAM64";
	case DTAPI_MOD_QAM256:		return "QAM256";
	case DTAPI_MOD_ATSC:		return "ATSC";
	case DTAPI_MOD_DVBT:		return "DVB-T";
	case DTAPI_MOD_DVBS_QPSK:
	case DTAPI_MOD_DVBS_BPSK:	return "DVB-S";
	case DTAPI_MOD_DVBS2_QPSK:
	case DTAPI_MOD_DVBS2_8PSK:	return "DVB-S2";
	default:					return "?";
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::Bandwidth2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::Bandwidth2Str() const
{
	switch( m_Bandwidth )
	{
	case DTAPI_MOD_DVBT_6MHZ:	return "6MHz";
	case DTAPI_MOD_DVBT_7MHZ:	return "7MHz";
	case DTAPI_MOD_DVBT_8MHZ:	return "8MHz";
	default:					return "?";
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::IpProtocol2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::IpProtocol2Str() const
{
	switch ( m_IpPars.m_Protocol )
	{
	case DTAPI_PROTO_AUTO:		return "AUTO";
	case DTAPI_PROTO_UDP:		return "UDP";
	case DTAPI_PROTO_RTP:		return "RTP";
	default:					return "?";
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::LnbVoltage2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char* CommandLineParams::LnbVoltage2Str() const
{
	switch ( m_LnbVoltage )
	{
	case DTAPI_LNB_13V:	return "13V";
	case DTAPI_LNB_14V:	return "14V";
	case DTAPI_LNB_18V:	return "18V";
	case DTAPI_LNB_19V:	return "19V";
	default:			return "Disabled";
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::LnbTone2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::LnbTone2Str() const
{
	if (m_LnbToneEnable)
		return "22 kHz";
	else
		return "No Tone";
}

//.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams::DiseqcPortGroup2Str -.-.-.-.-.-.-.-.-.-.-.-.
//
const char* CommandLineParams::DiseqcPortGroup2Str() const
{
	switch ( m_DiseqcPortGroup )
	{
	case 0xF0:	return "OptA, PosA, V, Lo";
	case 0xF1:	return "OptA, PosA, V, Hi";
	case 0xF2:	return "OptA, PosA, H, Lo";
	case 0xF3:	return "OptA, PosA, H, Hi";
	case 0xF4:	return "OptA, PosB, V, Lo";
	case 0xF5:	return "OptA, PosB, V, Hi";
	case 0xF6:	return "OptA, PosB, H, Lo";
	case 0xF7:	return "OptA, PosB, H, Hi";
	case 0xF8:	return "OptB, PosA, V, Lo";
	case 0xF9:	return "OptB, PosA, V, Hi";
	case 0xFA:	return "OptB, PosA, H, Lo";
	case 0xFB:	return "OptB, PosA, H, Hi";
	case 0xFC:	return "OptB, PosB, V, Lo";
	case 0xFD:	return "OptB, PosB, V, Hi";
	case 0xFE:	return "OptB, PosB, H, Lo";
	case 0xFF:	return "OptB, PosB, H, Hi";
	default:	return "???";
	}
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Recorder implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::Recorder -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Recorder::Recorder() : m_pFile(NULL), m_pBuf(NULL), m_Demod(false), m_Ip(false)
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::~Recorder -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Recorder::~Recorder()
{
	// Detach from channel and device
	m_DtInp.SetRxControl(DTAPI_RXCTRL_IDLE);
	m_DtInp.Detach(DTAPI_INSTANT_DETACH);
	m_DtDvc.Detach();

	// Free our buffer
	if ( m_pBuf != NULL )
		delete [] m_pBuf;

	// donot forget to close our file
	if ( m_pFile != NULL )
		::fclose(m_pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::AttachToInput -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Recorder::AttachToInput()
{
	DTAPI_RESULT dr;
	int n, NumHwFuncsFound(0);
	DtHwFuncDesc HwFuncs[20];

	// Scan for available DekTec devices
	dr = ::DtapiHwFuncScan(sizeof(HwFuncs)/sizeof(HwFuncs[0]), NumHwFuncsFound, HwFuncs);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrDtapiHwScanFailed, ::DtapiResult2Str(dr) );

	// Loop through found hardware functions
	bool Found(false);
	int DeviceNum(0);
	DtHwFuncDesc* pHwf = &HwFuncs[0];
	for ( n=0; n<NumHwFuncsFound; n++, pHwf++ )
	{
		// Skip non-inputs
		if ( 0==(pHwf->m_ChanType & DTAPI_CHAN_INPUT) )
			continue;

		// Looking for a specific type??
		if (    pHwf->m_DvcDesc.m_TypeNumber == m_CmdLineParams.m_DvcType
		     && pHwf->m_Port==m_CmdLineParams.m_Port )
			DeviceNum++;	// only count devices matching our search type
		else if ( m_CmdLineParams.m_DvcType == -1 )
			DeviceNum++;	// count every device

		if ( DeviceNum==m_CmdLineParams.m_DvcNum )
		{
			Found = true;
			break;
		}
	}
	// Did we find our device?
	if ( !Found && DeviceNum==0 )
	{
		// Could not find any matching input at all
		if (   (m_CmdLineParams.m_DvcType>=100 && m_CmdLineParams.m_DvcType<200)
			|| (m_CmdLineParams.m_DvcType>=2100 && m_CmdLineParams.m_DvcType<2200) )
			throw Exc(c_ErrNoDta1xxFound, m_CmdLineParams.m_DvcType,
				  m_CmdLineParams.m_Port );
		else if (   (m_CmdLineParams.m_DvcType>=200 && m_CmdLineParams.m_DvcType<300) )
			throw Exc(c_ErrNoDtu2xxFound, m_CmdLineParams.m_DvcType,
				  m_CmdLineParams.m_Port );
		else
			throw Exc(c_ErrNoInputFound);
	}
	else if ( !Found && m_CmdLineParams.m_DvcNum > DeviceNum )
	{
		// Could no find the Nth matching input
		if (   (m_CmdLineParams.m_DvcType>=100 && m_CmdLineParams.m_DvcType<200)
			|| (m_CmdLineParams.m_DvcType>=2100 && m_CmdLineParams.m_DvcType<2200) )
			throw Exc(c_ErrCouldNotFindDta1xxN, m_CmdLineParams.m_DvcType,
					  m_CmdLineParams.m_DvcNum, m_CmdLineParams.m_Port);
		else if (   (m_CmdLineParams.m_DvcType>=200 && m_CmdLineParams.m_DvcType<300) )
			throw Exc(c_ErrCouldNotFindDtu2xxN, m_CmdLineParams.m_DvcType,
					  m_CmdLineParams.m_DvcNum, m_CmdLineParams.m_Port);
		else
			throw Exc(c_ErrCouldNotFindInputN, m_CmdLineParams.m_DvcNum,
					  m_CmdLineParams.m_Port);
	}

	// Attach to the device
	dr = m_DtDvc.AttachToSerial(pHwf->m_DvcDesc.m_Serial);
	if ( dr == DTAPI_E_DRIVER_INCOMP )
	{
		// Special case: driver version is not compatible with this version of DtRecord,
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

	// Attach to the output channel
	dr = m_DtInp.AttachToPort(&m_DtDvc, pHwf->m_Port);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrFailToAttachToChan, ::DtapiResult2Str(dr));

	// Check for demodulator card
	m_Demod = (m_DtInp.m_HwFuncDesc.m_StreamType == DTAPI_TS_MOD);
	// Check for IP output
	m_Ip = (m_DtInp.m_HwFuncDesc.m_StreamType == DTAPI_TS_OVER_IP);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::DisplayPlayInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Recorder::DisplayPlayInfo()
{
	char sz[256];

	LogF("");
	LogF("Start recording");
	LogF("- Record file name      : %s", m_CmdLineParams.m_FileName);

	// Check for demodulator parameters
	if ( m_Demod )
	{
		LogF("- Modulation Type       : %s", m_CmdLineParams.ModType2Str() );
		LogF("- Carrier Frequency     : %.2f MHz", m_CmdLineParams.m_CarrierFreq );

		// QAM modulation parameters
		if (   m_CmdLineParams.m_ModType==DTAPI_MOD_QAM64
			|| m_CmdLineParams.m_ModType==DTAPI_MOD_QAM256 )
		{
			LogF("- J.83                  : %s", m_CmdLineParams.QamJ83Annex2Str() );
		}  
		// DVB-T modulation parameters
		if ( m_CmdLineParams.m_ModType == DTAPI_MOD_DVBT )
			LogF("- Bandwitdh             : %s", m_CmdLineParams.Bandwidth2Str() );

		// LNB and DiSEqC settings (only DTA-2137)
		if (m_DtDvc.TypeNumber() == 2137)
		{
			LogF("- LNB Voltage           : %s", m_CmdLineParams.LnbVoltage2Str() );
			LogF("- LNB Tone              : %s", m_CmdLineParams.LnbTone2Str() );
			if (m_CmdLineParams.m_DiseqcPortGroup != -1 )
				LogF("- DiSEqC Port Group     : %02X (%s)", 
										m_CmdLineParams.m_DiseqcPortGroup,
										m_CmdLineParams.DiseqcPortGroup2Str() );
		}
	}

	// Do we have a IP input
	if ( m_Ip )
	{
		// Log IP parameters
		DtapiIpAddr2Str( sz, sizeof(sz), m_CmdLineParams.m_IpPars.m_Ip );
		LogF("- IP Address            : %s:%d", sz, m_CmdLineParams.m_IpPars.m_Port );
		LogF("- Protocol              : %s", m_CmdLineParams.IpProtocol2Str() );
	}
	
	if ( m_CmdLineParams.m_MaxSize <= 0 )
		LogF("- Maximum file size     : No maximum set");
	else if ( m_CmdLineParams.m_MaxSize < 1024 )
		LogF("- Maximum file size     : %.1f MB", double(m_CmdLineParams.m_MaxSize));
	else
		LogF("- Maximum file size     : %.1f GB", double(m_CmdLineParams.m_MaxSize/1024.0) );
	
	LogF("- Receive Mode          : %s", m_CmdLineParams.RxMode2Str() );
	
	if ( m_DtDvc.Category() == DTAPI_CAT_USB )
		sprintf(sz, "DTU-%d", m_DtDvc.TypeNumber() );
	else /*if ( m_DtDvc.Category() == DTAPI_CAT_PCI )*/
		sprintf(sz, "DTA-%d", m_DtDvc.TypeNumber() );
	LogF("- Input device          : %s port %d (#%d)", sz, m_CmdLineParams.m_Port, 
		 m_CmdLineParams.m_DvcNum);

#ifdef WIN32
	Log("");
	Log("Press any key to stop recording");
	Log("");
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::InitInput -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Recorder::InitInput()
{
	DTAPI_RESULT dr;

	// Init channel to initial 'safe' state
	dr = m_DtInp.SetRxControl(DTAPI_RXCTRL_IDLE);	// Start in IDLE mode
	if ( dr != DTAPI_OK )
		throw Exc( c_ErrFailSetRxControl, ::DtapiResult2Str(dr) );
	dr = m_DtInp.ClearFifo();			// Clear FIFO (i.e. start with zero load)
	dr = m_DtInp.ClearFlags(0xFFFFFFFF);// Clear all flags

	// Apply IP settings (if we have a IP output)
	if ( m_Ip )
	{
		dr = m_DtInp.SetIpPars( &m_CmdLineParams.m_IpPars );
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetIpPars, ::DtapiResult2Str(dr) );
	}
	
	// Set the receive mode
    dr = m_DtInp.SetRxMode( m_CmdLineParams.m_RxMode );
	if ( dr != DTAPI_OK )
		throw Exc( c_ErrFailSetRxMode, ::DtapiResult2Str(dr) );

	// Set the polarity mode (for ASI interface only)
	if (m_CmdLineParams.m_Polarity!=-1 && m_DtInp.m_HwFuncDesc.m_StreamType==DTAPI_ASI_SDI && m_DtInp.m_HwFuncDesc.m_DvcDesc.m_TypeNumber!=225)
	{
		dr = m_DtInp.PolarityControl( m_CmdLineParams.m_Polarity );
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetPolarCtrl, ::DtapiResult2Str(dr) );
	}

	// Apply de-modulation settings (if we have a de-modulator)
	if ( m_Demod )
	{
		__int64 Freq = (__int64)(m_CmdLineParams.m_CarrierFreq*1E6);
		dr = m_DtInp.SetTunerFrequency( Freq );
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetRfControl, ::DtapiResult2Str(dr) );

		// Set modulation control
		if (    m_CmdLineParams.m_ModType==DTAPI_MOD_QAM64 
			 || m_CmdLineParams.m_ModType==DTAPI_MOD_QAM256 )
		{
			dr = m_DtInp.SetDemodControl( m_CmdLineParams.m_ModType,
										m_CmdLineParams.m_QamJ83Annex,
										-1, -1);
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_ATSC )
		{
			dr = m_DtInp.SetDemodControl( m_CmdLineParams.m_ModType,
										  DTAPI_MOD_ATSC_VSB8, -1, -1);
		}
		else if ( m_CmdLineParams.m_ModType==DTAPI_MOD_DVBT )
		{
			int XtraPar1 =    m_CmdLineParams.m_Bandwidth;
			dr = m_DtInp.SetDemodControl( m_CmdLineParams.m_ModType,
										  0, XtraPar1, -1);
		}
		else if (    m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS_QPSK
				  || m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS_BPSK
				  || m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_QPSK
				  || m_CmdLineParams.m_ModType==DTAPI_MOD_DVBS2_8PSK )
		{
			dr = m_DtInp.SetDemodControl( m_CmdLineParams.m_ModType,
										  -1, -1, -1);
		}
		if ( dr != DTAPI_OK )
			throw Exc( c_ErrFailSetDemodControl, ::DtapiResult2Str(dr) );


		// Apply LNB and DiSEqC settings (only for DTA-2137)
		if (m_DtDvc.TypeNumber() == 2137)
		{
			if (m_CmdLineParams.m_LnbVoltage != -1)
				dr = m_DtInp.LnbSetVoltage(m_CmdLineParams.m_LnbVoltage);
			
			if (m_CmdLineParams.m_LnbVoltage==-1 && !m_CmdLineParams.m_LnbToneEnable)
				dr = m_DtInp.LnbEnable(false);
			else
				dr = m_DtInp.LnbEnable(true);

			dr = m_DtInp.LnbEnableTone(m_CmdLineParams.m_LnbToneEnable);

			if (m_CmdLineParams.m_DiseqcPortGroup != -1 )
			{
				dr = m_DtInp.LnbEnable(true);
				unsigned char  DiseqcStandByOffCmd[] = {0xE0, 0x10, 0x03 };
				dr = m_DtInp.LnbSendDiseqcMessage(DiseqcStandByOffCmd, sizeof(DiseqcStandByOffCmd));

#ifdef WIN32
				Sleep(100);
#else
				usleep(100000);
#endif 

				unsigned char  DiseqcPortGroupCmd[] = {0xE0, 0x10, 0x038, 0x00 };
				DiseqcPortGroupCmd[3] = m_CmdLineParams.m_DiseqcPortGroup;
				dr = m_DtInp.LnbSendDiseqcMessage(DiseqcPortGroupCmd, sizeof(DiseqcPortGroupCmd));
			}
		}
	}
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::Log -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Recorder::Log(const char* pMessage,  bool IgnoreSilence)
{
    // Check for silent mode
	if ( !IgnoreSilence && m_CmdLineParams.m_SilentMode )
		return;
	// Print message and add new line
	printf(pMessage); printf("\n");
}
// Overload with optional parameters
void Recorder::LogF(const char* pMessage, ... )
{
	char sz[512];

	va_list  ArgList;
	va_start(ArgList, pMessage);
	_vsnprintf(sz, sizeof(sz)-1, pMessage, ArgList);
	va_end(ArgList);

	Log(sz, false);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::RecordFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void Recorder::RecordFile()
{
	DTAPI_RESULT dr;
	int SdiMode=-1;
	bool ValidSignal = false, HwFifoOvf=false;
	int FifoLoad, NumToRead;
	__int64 NumBytesStored = 0;		// Number bytes stored in file

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- First loop wait for a signal -.-.-.-.-.-.-.-.-.-.-.-.-.-

#ifdef WIN32
	while ( !_kbhit() && !ValidSignal) {
#else
	while ( !ValidSignal ){
#endif
		int CarrierDetect, n;
		dr = m_DtInp.GetStatus(SdiMode, n, CarrierDetect, n, n, n);

		// Do we have a carrier and for SDI recording we also check the SDI mode
		if (     CarrierDetect != DTAPI_CLKDET_OK
 			|| ( m_CmdLineParams.m_RecDtSdiFile && SdiMode == DTAPI_SDIMODE_INV ) )
		{
			// No carrier => wait shortly for signal
#ifdef WIN32
			Sleep(500);
#else
			usleep(500000);
#endif 
			continue;
		}
		ValidSignal = true;
	}
	if ( !ValidSignal )
		return;	// No signal found i.e. user must have pressed a key to exit loop

	// For SDI mode we need to add a dtsdi file header
	if ( m_CmdLineParams.m_RecDtSdiFile )
	{
		// Create header
		DtSdiFileHdr FileHdr;

		FileHdr.m_MagicCode1 = DTSDI_MAGIC_CODE1;
		FileHdr.m_MagicCode2 = DTSDI_MAGIC_CODE2;
		FileHdr.m_MagicCode3 = DTSDI_MAGIC_CODE3;
		FileHdr.m_FmtVersion = DTSDI_FMT_VERSION;

		// Set data type in header
		if ( SdiMode == DTAPI_SDIMODE_525 ) 
			FileHdr.m_DataType = DTSDI_TYPE_SDI_525;
		else
			FileHdr.m_DataType = DTSDI_TYPE_SDI_625;

		// Add flags
		FileHdr.m_Flags = 0;
		// FULL or active video mode?
		if ( (m_CmdLineParams.m_RxMode & (DTAPI_RXMODE_SDI | 0x7)) == DTAPI_RXMODE_SDI_FULL )
			FileHdr.m_Flags = DTSDI_SDI_FULL;
		else
			FileHdr.m_Flags = DTSDI_SDI_ACTVID;
		// 8 or 10-bit?
		if ( 0!=(m_CmdLineParams.m_RxMode & DTAPI_RXMODE_SDI_10B) )
			FileHdr.m_Flags |= DTSDI_SDI_10B;
		// Huffman compressed?
		if ( 0!=(m_CmdLineParams.m_RxMode & DTAPI_RXMODE_SDI_HUFFMAN) )
			FileHdr.m_Flags |= DTSDI_SDI_HUFFMAN;


		// Write header to file
		int NumBytesWritten = (int)fwrite(&FileHdr, 1, sizeof(FileHdr), m_pFile);
		// Sanity check
		if ( NumBytesWritten != sizeof(FileHdr) )
			throw Exc(c_ErrFailWriteDtSdiFileHdr);
	}

	// Init allignment
	if ( (m_CmdLineParams.m_RxMode&0x7)==DTAPI_RXMODE_ST204 )
    {   
		m_Allign = 204;		// Allign on multiples of 204 bytes
        // Do we need to account for timestamps?
        if ( (m_CmdLineParams.m_RxMode & DTAPI_RX_TIMESTAMP)!=0 ) 
            m_Allign += 4;
    }
	else if ( (m_CmdLineParams.m_RxMode&0x7)==DTAPI_RXMODE_STRAW )
		m_Allign = 4;		// Allign on multiples of 4 bytes
	else if ( (m_CmdLineParams.m_RxMode&0x7)==DTAPI_RXMODE_STTRP )
		m_Allign = 212;		// Allign on multiples of 212 bytes (TRP-packet + timestamp)
	else
    {
		m_Allign = 188;		// Allign on multiples of 188 bytes
        // Do we need to account for timestamps?
        if ( (m_CmdLineParams.m_RxMode & DTAPI_RX_TIMESTAMP32)!=0 ) 
            m_Allign += 4;
    }

	// Start reception
	dr = m_DtInp.SetRxControl(DTAPI_RXCTRL_RCV);
	if ( dr != DTAPI_OK )
		throw Exc(c_ErrFailSetRxControl, ::DtapiResult2Str(dr));


	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Second loop record data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#ifdef WIN32
	while ( !_kbhit() ) {
#else
	while ( 1 ) {
#endif

		// Get FIFO load. Continue if too few bytes available in receive FIFO
		dr = m_DtInp.GetFifoLoad(FifoLoad);
		if ( FifoLoad < c_BufSize+512 )
		{
			// Sleep 1ms, before checking load again
		#ifdef WIN32
			Sleep(1);
		#else
			usleep(1000);
		#endif 
			continue;
		}

		if ( m_CmdLineParams.m_RecDtSdiFile )
		{
			// For SDI we use the read frame method, automaticaly alligns on
			// frame boundaries
			NumToRead = c_BufSize;
			dr = m_DtInp.ReadFrame( (unsigned int*)m_pBuf, NumToRead );
		}
		else
		{
			// Read alligned number of data from input channel
			NumToRead = (c_BufSize / m_Allign) * m_Allign;
			dr = m_DtInp.Read( m_pBuf, NumToRead );
		}
		if ( dr != DTAPI_OK )
			throw Exc(c_ErrFailRead, ::DtapiResult2Str(dr) );

		// Compute number of bytes left to store
		__int64 NumLeftToStore;
		if ( m_CmdLineParams.m_MaxSize <=0 )
			NumLeftToStore = NumToRead+1;	// No max => we can store all
		else
			NumLeftToStore = (m_CmdLineParams.m_MaxSize * 1024 * 1024) - NumBytesStored;

		// Compute number of bytes to write to file
		int NumToWrite=0;

		if ( m_CmdLineParams.m_RecDtSdiFile )
		{
			// For SDI, data is alligned to frames => write all data
			NumToWrite = NumToRead;
		}
		else
		{
			// Account our allignment requirements
			NumLeftToStore = ( NumLeftToStore/m_Allign ) * (__int64)(m_Allign+1);

			NumToWrite = ( NumLeftToStore >= NumToRead ) ? NumToRead : (int)NumLeftToStore;
		}
				
		// Write data to our file
		if ( NumToWrite != ::fwrite(m_pBuf, sizeof(char), NumToWrite, m_pFile) )
			throw Exc(c_ErrWriteFile);

		// Update number stored
		NumBytesStored += NumToWrite;

		// Should we stop?
		NumLeftToStore -= NumToWrite;
		if ( NumLeftToStore <= 0 )
			break;	// Reached max size => exit record loop

		// Check for overflow (only report overflow once)
		int Flags=0, Latched=0;
		dr = m_DtInp.GetFlags(Flags, Latched);
		if ( 0!=(Latched & DTAPI_RX_FIFO_OVF) && !HwFifoOvf )
		{
			LogF(c_WarnHwFifoOverflow);
			HwFifoOvf = true;	// Only log warning once
		}
		dr = m_DtInp.ClearFlags(Latched);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::Record -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int Recorder::Record(int argc, char* argv[])
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
			Log("Unknown exception encountered. Exiting application", true);
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

		LogF("DtRecord - Recording Utility V%d.%d.%d (c) 2000-2010 DekTec Digital Video B.V.\n",
			 DTRECORD_VERSION_MAJOR, DTRECORD_VERSION_MINOR, DTRECORD_VERSION_BUGFIX );


		//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open the record file -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

		// Try to open the file for writing
		m_pFile = fopen( m_CmdLineParams.m_FileName, "wb");
		if ( m_pFile == NULL )
			throw Exc(c_ErrFailToOpenFile, m_CmdLineParams.m_FileName);

		//-.-.-.-.-.-.-.-.-.-.-.-.- Attach to the output channel -.-.-.-.-.-.-.-.-.-.-.-.-

		AttachToInput();

		//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Final initialisation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

		// Init output
		InitInput();

		// Create our transfer buffer
		m_pBuf = new char [c_BufSize];

		//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start playing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

		// Print start info
		DisplayPlayInfo();
		// Loop file
		RecordFile();
	}
	catch( Exc e )
	{
        Log(e, true);
		RetValue = -1;
	}

	// Detach from channel and device
	m_DtInp.SetRxControl(DTAPI_RXCTRL_IDLE);
	m_DtInp.Detach(DTAPI_INSTANT_DETACH);
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Recorder::ShowHelp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void Recorder::ShowHelp()
{
	Log("" );
	Log("Usage:");
	Log( "   DtRecord recfile [-t type] [-n number] [-i port] [-x maxsize]\n" \
		 "          [-mt type] [-mf freq] [-ma annex] [-mb bandwidth]\n" \
		 "          [-lnbv voltage] [-lnbt] [-diseqcpg port_group_data_byte]\n" \
         "          [-ipa ip_address_pair] [-ipp protocol] [-pc polarity] [-s] [-?]");
	Log("");
	Log("Where:");
	Log("   recfile is the name of the record file");
	Log("");
	Log("NOTE: It is recommended to use the '.dtsdi' extension for SDI record files");
	Log("");
	Log("Options:");
	Log("   -t  Device type to use (default: any input device)");
	Log("         Use: 112  (for DTA-112)");
	Log("              115  (for DTA-115)");
	Log("              116  (for DTA-116)");
	Log("              117  (for DTA-117)");
	Log("              122  (for DTA-122)");
	Log("              124  (for DTA-124)");
	Log("              140  (for DTA-140)");
	Log("              145  (for DTA-145)");
	Log("              160  (for DTA-160)");
	Log("              2135 (for DTA-2135)");
	Log("              2137 (for DTA-2137)");
	Log("              2144 (for DTA-2144)");
	Log("              2145 (for DTA-2145)");
	Log("              225  (for DTU-225)");
	Log("              235  (for DTU-235)");
	Log("              245  (for DTU-245)");
	Log("              545  (for DTA-545)");
	Log("   -n  Device number to use (default: 1)");
	Log("   -i  Port number of the input channel to use (default: 1) ");
	Log("   -m  Receive Mode (default: ST188)");
	Log("         Use: ST188         Store packets as 188-byte packets");
	Log("              ST204         Store packets as 204-byte packets");
	Log("              STRAW         No notion of packets. Store all valid bytes");
    Log("              STTRP		 Store in transparent-mode-packet + timestamp format");
    Log("              ST188T        Store packets as 188-byte packets + timestamp");
    Log("              ST204T        Store packets as 204-byte packets + timestamp");
	Log("              SDI8B         Store complete SDI frame, in 8-bit format");
	Log("              SDI10B        Store complete SDI frame, in 10-bit format");
	Log("              SDI8B_ACTVID  Store active video only, in 8-bit format");
	Log("              SDI10B_ACTVID Store active video only, in 10-bit format");
	Log("   -c  Huffman compression (default: OFF)");
	Log("         Use: OFF  Disable Huffman compression");
	Log("              ON   Enable Huffman compression");
	Log("              ON   Enable Huffman compression");
	Log("   NOTE: Huffman compression can only be used with SDI modes");
	Log("   -mt  Modulation type (default: QAM64)");
	Log("         Use:  QAM64  (for QAM-64 modulation)");
	Log("               QAM256 (for QAM-256 modulation)");
	Log("               ATSC   (for ATSC (8-VSB) modulation)");
	Log("               DVBT   (for DVB-T (COFDM) modulation)");
	Log("               DVBS   (for DVB-S (QPSK or BSPSK) modulation)");
	Log("               DVBS2  (for DVB-S2 (QPSK or 8PSK) modulation)");
	Log("   -mf  Modulation carrier frequency in MHz (default: 578.0MHz)");
	Log("   -ma  J.83 Annex (default: Annex A");
	Log("         Use:  A   (for J.83 annex A (DVB-C))");
	Log("               B   (for J.83 annex B ('American QAM')");
	Log("               C   (for J.83 annex C ('Japanese QAM')");
	Log("   -mb  DVB-T bandwidth (default: 8MHz)");
	Log("         Use:  6   (for 6MHz)");
	Log("               7   (for 7MHz)");
	Log("               8   (for 8MHz)");
	Log("   -lnbv  LNB voltage (default: disabled)");
	Log("         Use:  13   (for 13V)");
	Log("               14   (for 14V)");
	Log("               18   (for 18V)");
	Log("               19   (for 19V)");
	Log("   -lnbt  LNB 22kHz tone (default: no tone)");
	Log("   -diseqcpg  issue DiSEqC port group command");
	Log("          Use: F0 => OptA, PosA, V, Lo");
	Log("               F1 => OptA, PosA, V, Hi");
	Log("               F2 => OptA, PosA, H, Lo");
	Log("               F3 => OptA, PosA, H, Hi");
	Log("               F4 => OptA, PosB, V, Lo");
	Log("               F5 => OptA, PosB, V, Hi");
	Log("               F6 => OptA, PosB, H, Lo");
	Log("               F7 => OptA, PosB, H, Hi");
	Log("               F8 => OptB, PosA, V, Lo");
	Log("               F9 => OptB, PosA, V, Hi");
	Log("               FA => OptB, PosA, H, Lo");
	Log("               FB => OptB, PosA, H, Hi");
	Log("               FC => OptB, PosB, V, Lo");
	Log("               FD => OptB, PosB, V, Hi");
	Log("               FE => OptB, PosB, H, Lo");
	Log("               FF => OptB, PosB, H, Hi");
	Log("   -ipa IP address/port (e.g. 192.168.0.1[:5768], port is optional)");
	Log("   -ipp IP Protocol (default: AUTO)");
	Log("         Use:  AUTO (for AUTO detect)");
	Log("               UPD  (for UDP)");
	Log("               RTP  (for RTP)");
	Log("   -pc  Polarity Control (default: AUTO; in raw-mode: NORMAL )");
	Log("         Use:  AUTO   (for AUTO detect)");
	Log("               NORMAL (for Normal mode)");
	Log("               INVERT (for Inverted mode)");
	Log("   -s  Silent mode. No messages printed");
	Log("   -?  Display this help");
	Log("");
	Log("NOTE: The first option can only be the recfile or the help option");
	Log("");
	Log("Examples:");
	Log("   DtRecord myfile.ts -x 10");
	Log("   DtRecord myfile.ts -t 124 -i 2 -m ST204");
	Log("   DtRecord mysdifile.dtsdi -t 245 -m SDI10B -c ON");
	Log("");
	LogF("DtRecord version: %d.%d.%d\n", DTRECORD_VERSION_MAJOR, DTRECORD_VERSION_MINOR,
		 DTRECORD_VERSION_BUGFIX);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Application entry point +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- main -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int main(int argc, char* argv[])
{
	int RetValue(0);
	Recorder TheRecorder;
	try
	{
		RetValue = TheRecorder.Record(argc, argv);
	}
	catch(...)
	{
		RetValue = -1;
	}
	return RetValue;
}
