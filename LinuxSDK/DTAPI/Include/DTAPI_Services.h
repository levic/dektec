// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DTAPI_Services.h *#*#*#*#*#*#*#*#*#* (C) 2021 DekTec
//
// This header files defines a number of classes and structs to interface with "services"
// running in DTAPI or in the DTAPI service.
// 
// Services available in the current version of DTAPI:
//  1. PTP clock slave              // Management and control of an IEEE 1588 clock slave 
//
// Interfacing with a service is based on "parameters". A parameter can be:
//  - A configuration setting;
//  - A status variable;
//  - A measurement value.
// 
// Within each service, parameters are organized in groups. A group is identified by a
// string, e.g. "General". Each parameter within a group is identified by its name, which
// also is a string, e.g. "DelayMechanism". 
// The full name of a parameter is e.g. "General.DelayMechanism".
//
// The group and parameter-name strings are "well known" and remain the same with each
// new version of DTAPI. Parameters can be read and written (if 'Writable') based on
// an integer parameter ID. IDs can change between DTAPI versions. Therefore, parameter
// IDs need to be requested from the service with Proxy::GetParDescs().
//
// For further explanation, and for a tutorial, see "DTAPI Reference - PTP.pdf".

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Dtapi
{

// Avoid having to use std:: in the Dtapi namespace.
using std::unique_ptr;
using std::string;
using std::vector;

namespace Service
{

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- enum class Service -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Enumerates the services available in DTAPI.
//
enum class Services
{
    // PTP clock slave.
    // This service is available on the 10GbE port of the DTA-2110. 
    PtpClockSlave
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct Variant -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// As we cannot use C++ 17 type variant in DTAPI (to support older compilers), this is
// the DTAPI emulation of a simple variant type that can hold a double, int or a string.
// Variants are used to pass parameter values between an application and a service
// running in DTAPI.
// 
// Complex parameter values are encoded in strings. Functions are available to unmarshal
// such strings to a C struct for easy programmatic usage:
//  vector<Ptp::IpAddress> Ptp::ToIpAddress(const string&)
//  vector<Ptp::MasterInfo> Ptp::ToMasterInfo(const string&)
// 
struct Variant
{
    // Types that can be stored in a Variant.
    enum class Type {Empty, Double, Int, UInt64, Bool, String};

    // Get Variant's value. Available for T=int, double and string.
    template<typename T> T GetVal() const;
    
    // Convert variant to a human readable string.
    string ToString() const;

    // Default constructor creates an empty Variant.
    Variant()=default;

    // Overloaded constructors for convenient in-place construction.
    // This allows initialization of a Variant with e.g. {1.72}.
    Variant(double DoubleVal);
    Variant(int IntVal);
    Variant(uint64_t UIntVal);
    Variant(bool BoolVal);
    Variant(string StringVal);
    const Type GetType() const;

private:
    Type VarType{Type::Empty};
    string Data;                    // Variant value is encoded in here.
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct ParIdValue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Parameter ID with parameter value.
//
struct ParIdValue
{
    int ParId{-1};                  // Obtained with Proxy::GetParDescs().
    Variant Value;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct EnumValDesc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Enumerator-value descriptor. Describes a single value in an enumeration.
//
struct EnumValDesc
{
    int IntVal;                     // Corresponding integer value.
    string Identifier;              // Enumeration value as a string, e.g. "EndToEnd".
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct ParDesc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Parameter metadata that describes the properties of a parameter.
// Parameter descriptors can be obtained by querying a service proxy that is attached
// to the service using Proxy::GetParDescs().
//
struct ParDesc
{
    // Unique integer identifying the parameter.
    // Runs from 0 to the number of parameters supported by the service.
    int ParId{-1};

    // Parameters are known by their name, e.g. "ClockJumpThreshHold".
    string Name;

    // The parameter description is purely intended for humans.
    string Description;

    // Enumeration descriptor.
    // If this vector is not empty, the parameter is an enumeration and the parameter
    // type is Variant::Type::Int.
    vector<EnumValDesc> EnumDesc;

    // Parameter properties.
    Variant Min, Max, Default;      // Empty for strings.
    Variant::Type Type{Variant::Type::Empty};
    bool Writable{false};           // Will SetPar(.) for the parameter succeed?
};

int GetParId(const vector<ParDesc>&, const string& ParName);

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class Proxy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Class to communicate with a DTAPI service, e.g. the PTP clock slave service.
//
class Proxy
{
public:
    // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General methods -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Attach to a given DTAPI service, for a given device and port number (1-based).
    // PRECONDITION: The device must be attached to the hardware.
    void Attach(const class DtDevice&, int PortNo, Services, bool Exclusive=true);

    // Detach from the DTAPI service to which this proxy was attached.
    void Detach();

    // Method to persist the current settings as the initial settings for the service
    // the next time it is restarted.
    void SaveCurrentSettingsAsStartupValues();
    
    // -.-.-.-.-.-.-.-.-.-.- Functions for querying service metadata -.-.-.-.-.-.-.-.-.-.-

    // Get available parameter groups for this DTAPI service.
    // Groups are identified by a string, which can be used in GetParDescs to obtain
    // the descriptors for the parameters in that group.
    vector<string> GetParGroups() const;

    // Get parameter descriptors for a given group.
    vector<ParDesc> GetParDescs(const string& ParGroup) const;

    // .-.-.-.-.-.-.-.-.-.- Functions for getting/setting parameters -.-.-.-.-.-.-.-.-.-.-

    // Get/set parameter functions operating on a single parameter.
    Variant GetParVal(int ParId) const;
    void SetParVal(int ParId, Variant ParVal);

    // Get/set parameter functions operating on multiple parameters in one go.
    vector<ParIdValue> GetParVals(const vector<int>& ParIds) const;
    void SetParVals(const vector<ParIdValue>& ParIdVals);
    Proxy();
    virtual ~Proxy();
protected:
    unique_ptr<class SrvcsClient> SrvcsClient;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Specialized exception thrown by the functions defined in this header file. 
//
struct Exc : public std::exception
{
    enum class Reason
    {
        DeviceNotAttached, DeviceAlreadyAttached, InvalidParId, InvalidPortNo,
        ServiceAlreadyInUse, TypeMismatch, InvalidParType, ParOutOfRange,
        UnknownParGroup, UnkwownParId, UnknownParName, UnknownService,
        InvalidSize, InvalidCmd, OutOfMemory, Unknown, InvalidXmlString,
        IncompatibleService, ConnectToService, NotImplemented, ExclusiveInUse, UnknownGroupName,
        NotExclusiveAccess, ReadOnlyPar, DriverIncompatible, ErrorAttachingDevice, DeviceNotFound,
        NotSupported, NoAdapterIpAddr, MulticastJoin, ProtocolBind, PtpTimestamping, InvalidState, PtpSend
    } TheReason = Reason::Unknown;
    Exc(Reason Reason) : TheReason(Reason){ }
};

} // namespace Service

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ namespace Ptp +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

namespace Ptp
{
    // +=+=+=+=+=+ Enumerations used for managing the PTP-clock-slave service +=+=+=+=+=+=
 
    // For a description of the enumeration values, refer to "DTAPI Reference - PTP.pdf".

    // Enumeration used for parameter "General.DelayMechanism".
    enum class DelayMechanism
    {
        Auto, EndToEnd, PeerToPeer
    };

    // ...
    enum class SlaveState
    {
        Disabled, Initializing, Listening, Faulty, Uncalibrated, Slave
    };
    enum class MasterLockingStatus
    {
        NotInUse=0,
        FreeRun=1,
        ColdLocking=2,
        WarmLocking=3,
        Locked=4
    };
    enum class LeapSecondJump
    {
        Other = 0,
        LeapSeconds = 1
    };

    enum class IpV6Scope
    {
        InterfaceLocal = 1,
        LinkLocal = 2,
        AdminLocal = 4,
        SiteLocal = 5, 
        OrganizationLocal = 8,
        Global = 14
    };
    enum class NetworkProtocol
    {
        IpV4,
        IpV6
    };


    // .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct MasterInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    //
    // PTP clock master information, as collected by the PTP clock slave. 
    // Please refer to "DTAPI Reference - PTP.pdf" for a field-by-field explanation of
    // the fields in this structure.
    // To obtain a vector of MasterInfo structs, get parameter "General.MasterInfo" from
    // the PTP clock slave service as a string, and apply function ToMasterInfo(.).
    //
    struct MasterInfo
    {
        bool AlternateMaster{}, FrequencyTraceable{}, Leap59{}, Leap61{}, TimeTraceable{};
        bool CurrentUtcOffsetValid{};
        int AnnounceInterval{}, AnnounceMsg{}, CurrentLocalOffset{}, DomainNumber{};
        int GrandmasterClockAccuracy{}, GrandmasterClockClass{};
        int MajorSdoId{}, MinorSdoId{}, OffsetScaledLogVariance{};
        int GrandmasterPriority1{}, GrandmasterPriority2{}, ParentPortNumber{};
        int StepsRemoved{}, TimeSource{};
        string OriginTimestamp{};
        string IPAddress{};
        uint64_t ParentPortIdentity{}, GrandmasterIdentity{};

        bool operator==(const MasterInfo& Oth) const
        {
            if (this == &Oth)
                return true;

            return (AlternateMaster==Oth.AlternateMaster
                                 && FrequencyTraceable==Oth.FrequencyTraceable
                                 && Leap59==Oth.Leap59
                                 && Leap61==Oth.Leap61
                                 && TimeTraceable==Oth.TimeTraceable
                                 && CurrentUtcOffsetValid==Oth.CurrentUtcOffsetValid
                                 && AnnounceInterval==Oth.AnnounceMsg
                                 && CurrentLocalOffset==Oth.CurrentLocalOffset
                                 && DomainNumber==Oth.DomainNumber
                                 && GrandmasterClockAccuracy==Oth.GrandmasterClockAccuracy
                                 && GrandmasterClockClass==Oth.GrandmasterClockClass
                                 && MajorSdoId==Oth.MajorSdoId
                                 && MinorSdoId==Oth.MinorSdoId
                                 && OffsetScaledLogVariance==Oth.OffsetScaledLogVariance
                                 && GrandmasterPriority1==Oth.GrandmasterPriority1
                                 && GrandmasterPriority2==Oth.GrandmasterPriority2
                                 && ParentPortNumber==Oth.ParentPortNumber
                                 && StepsRemoved==Oth.StepsRemoved
                                 && TimeSource==Oth.TimeSource
                                 && OriginTimestamp==Oth.OriginTimestamp
                                 && IPAddress==Oth.IPAddress
                                 && ParentPortIdentity==Oth.ParentPortIdentity
                                 && GrandmasterIdentity==Oth.GrandmasterIdentity);
        }
        bool operator!=(const MasterInfo& Oth) const
        {
            return !(*this == Oth);
        }
    };

    // Function to unmarshal the string obtained by a GetPar of "General.MasterInfo"
    // into a vector of master info objects.
    vector<MasterInfo> ToMasterInfo(const string&);

} // namespace PTP

} // namespace Dtapi