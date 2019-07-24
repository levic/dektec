//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPlay.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2006 DekTec
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  2006.10.08  MG  Created 

#ifndef __DTPLAY_H
#define __DTPLAY_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <stdarg.h>
#include "DtOpt.h"

// Windows/Microsoft uses with _ => Linux uses without
#ifndef WINBUILD
#define _vsnprintf vsnprintf
#define _vsnwprintf vswprintf
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- PCAP-file -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define PCAP_MAGIC_NUMBER_US     0xa1b2c3d4
#define PCAP_MAGIC_NUMBER_NS     0xa1b23c4d
#define PCAP_VERSION_MAJOR      2
#define PCAP_VERSION_MINOR      4

struct PcapFileHeader
{
    unsigned int  m_MagicNumber;        // Magic number
    unsigned short  m_VersionMajor;     // Major version number
    unsigned short  m_VersionMinor;     // Minor version number
    unsigned int  m_ThisZone;           // GMT to local correction; Always 0
    unsigned int  m_SigFigs;            // Accuracy of timestamps; Always 0
    unsigned int  m_SnapLen;            // Maximum length of captured packets, in bytes
    unsigned int  m_Network;            // Data link type
};

struct TimeVal
{
  int m_Seconds;
  int m_NsOrUs;
};
struct PcapPckHeader
{
    TimeVal  m_TimeStamp;               // Timestamp
    unsigned int  m_InclLen;            // Number of bytes saved in file
    unsigned int  m_OrigLen;            // Original length of packet
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class Exc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class Exc
{
public:
    Exc( const char* pFormat, ...  )
    {
        va_list  ArgList;
        va_start(ArgList, pFormat);
        _vsnprintf(m_ErrorMsg, sizeof(m_ErrorMsg)-1, pFormat, ArgList);
        va_end(ArgList);
    };
    virtual ~Exc() {};

    operator const char* () const { return m_ErrorMsg; }

protected:

    // Error message
    char m_ErrorMsg[1024];
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CommandLineParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class CommandLineParams
{
public:
    CommandLineParams();
    virtual ~CommandLineParams();

    void  Init();
    void  ParseCommandLine(int argc, char* argv[]);
    const char* TxMode2Str() const;
    const char* ModType2Str() const;
    const char* CodeRate2Str() const;
    const char* QamJ83Annex2Str() const;
    const char* Constellation2Str() const;
    const char* OfdmBandwidth2Str() const;
    const char* OfdmGaurdItv2Str() const;
    const char* OfdmTxMode2Str() const;
    const char* DvbS2Pilots2Str() const;
    const char* DvbS2FecFrameLength2Str() const;
    const char* DtmbFrameHdrMode2Str() const;
    const char* IqInterpFilter2Str() const;
    const char* IpProtocol2Str() const;

    //---- Application parameters ----

    wstring  m_FileName;        // Name of play file
    DtOpt  m_LoopCnt;           // Number of times to loop file


    DtOpt  m_DvcType;           // -t: Type of device 
    DtOpt  m_DvcNum;            // -n: Device number
    DtOpt  m_Port;              // -i: Port number of channel to use
    DtOpt  m_DblBuff;           // -db: use doubly buffered output

    DtOpt  m_TxMode;            // -m: Transmssion mode
    int  m_SdiSubValue;         // Subvalue for SDI IoConfig, autodeteced.
    DtOpt  m_Stuffing;          // -mS: ASI/SDI stuffing
    DtOpt  m_TxRate;            // -r: Transmission rate
    DtOpt  m_DataPid;           // -dP: PID of the data stream (T2MI/CCMB)
    DtOpt  m_DataPid2;          // -dP2: Second PID of the data stream (T2MI)

    DtOpt  m_ModType;           // -mt: Master modulation type
    DtOpt  m_CarrierFreq;       // -mf: carrier frequency
    DtOpt  m_OutpLevel;         // -ml: Modulator output level
    DtOpt  m_CodeRate;          // -mc: code rate
    DtOpt  m_QamJ83Annex;       // -ma: J83 annex (QAM-A/B/C)
    DtOpt  m_SpecInvers;        // -msi: Spectral inversion

    DtOpt  m_IqInterpFilter;    // -if: Interpolation filter to use in IQ mode

    DtOpt  m_Snr;               // -snr: signal-to-noise ration (-1.0 is no noise)

    DtOpt  m_Constellation;     // -mC: constellation
    DtOpt  m_Bandwidth;         // -mB: bandwidth
    DtOpt  m_OfdmTxMode;        // -mT: transmision mode
    DtOpt  m_OfdmGuardItv;      // -mG: guard interval

    DtOpt  m_DvbS2GoldSeqInit;  // -mI: DVB-S2 gold sequence initialisation value
    DtOpt  m_DvbS2Pilots;       // -mP: DVB-S2 pilots
    DtOpt  m_DvbS2FecFrameLength; // -mF: DVB-S2 FEC frame length

    DtOpt  m_DtmbFrameHdrMode;  // -mH: DTMB frame header mode

    DtOpt  m_Ipa;               // -ipa: IP address and IP port
    DtOpt  m_Ipp;               // -ipp: IP protocol
    DtOpt  m_Ipn;               // -ipn: Number of TPs per IP packet
    DtOpt  m_Ipt;               // -ipt: Time-to-live

    DtTsIpPars m_IpPars;        // IP parameters.
                                // -ipa: IP address and IP port
                                // -ipp: IP protocol
                                // -ipn: Number of TPs per IP packet

    DtOpt  m_SilentMode;        // -s: Enable silent mode (i.e. no printf)
    DtOpt  m_ShowHelp;          // -?: show help

    bool m_PlayDtSdiFile;       // Play .dtsdi file
    bool m_PlayPcapFile;        // Play .pcap file

protected:

    void  ParseParam(char* pParam, bool Flag, bool First=false, bool Last=false);
    void  ParseParamFlag(char* pParam, bool First=false, bool Last=false);
    void  ParseParamNotFlag(char* pParam, bool First=false, bool Last=false);

};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Player -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
class Player
{
public:
    Player();
    virtual ~Player();

    //---- Public operations ----
    int Play(int argc, char* argv[]);

protected:

    enum PortType
    {
        PT_UNKNOWN,
        PT_ASI,
        PT_IP,
        PT_MOD,
        PT_SDI,
    };

    //---- Internal operation ----
    void AttachToOutput();
    void AutoDetectSdiFormat();
    void DetectPcapFormat();
    void DisplayPlayInfo();
    bool HasOutputPort();
    void InitIsdbtPars(DtIsdbtPars& IsdbtPars);
    void InitOutput();
    bool IsMatchingPortType(PortType  WantedPortType, DtHwFuncDesc*  pHwFunc);
    PortType GuessPortTypeFromPars();
    void Log(const char* pMessage, bool IgnoreSilence=false);
    void Log(const wchar_t* pMessage, bool IgnoreSilence=false);
    void LogF(const char* pMessage, ... );
    void LogF(const wchar_t* pMessage, ... );
    void LoopFile();
    void ShowHelp();
    

    CommandLineParams  m_CmdLineParams;

    DtDevice  m_DtDvc;              // Our device
    DtOutpChannel  m_DtOutp;        // Our output channel
    bool  m_Modulator;              // Current output is a modulator
    bool  m_Ip;                     // Current output is a IP port
    FILE* m_pFile;                  // Our play file
    int  m_SizeOfDtSdiHdr;          // Size of DtSdi file header
    bool  m_m_PcapUsesNanoSeconds;  // If true PCAP-file timestamps in nanoseconds
                                    // otherwise microseconds
    bool  m_PcapEthernetLinkType;   // If true PCAP uses Ethernet linktype otherwise IP

    char* m_pBuf;                   // Our data buffer
    int  m_ExitLoad;                // Load at which we should exit our loop

};

#endif // #ifndef __DTPLAY_H
