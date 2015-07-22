//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtOpt.h *#*#*#*#*#*#*#*#*#*#*#*# (C) 2012 DekTec
//
// DtOpt - DekTec commandline options - header file

#ifndef __DTOPT_H
#define __DTOPT_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <string>
#include <list>
#include <cstdarg>
using namespace std;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. class DtOptException -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Exception class that will be thrown if there is any error during processing the
// commandline arguments.
class DtOptException
{
public:
    DtOptException(const wchar_t* pFormat, ...);
    virtual ~DtOptException() {};

    operator const wchar_t* () const { return m_ErrorMsg; }

protected:
    wchar_t m_ErrorMsg[1024];       // Error message
};


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- enum DtOptType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The type of an option.
enum DtOptType
{
    OPT_TYPE_INT,                   // Option is an integer
    OPT_TYPE_DOUBLE,                // Option is a double
    OPT_TYPE_BOOL,                  // Option is a boolean
    OPT_TYPE_STRING,                // Option is a string
    OPT_TYPE_INVALID,               // Invalid option
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtEnumOptPair -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Type used to provide a string->int mapping for integer types that have to be passed
// on the commandline as strings.
struct DtEnumOptPair
{
    wstring  m_Name;                // How is this item specified on the commandline
    int  m_Value;                   // The integer value of this item
    wstring  m_Description;         // Description of this item, used in PrintOptionsHelp
};

// Use this as last item in an array of DtEnumOptPair
#define OPT_PAIR_END {L"", -1, L""}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class DtOpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Representation of an internal option.
class DtOpt
{
public:
    DtOpt();
    operator int() const;

    // Assign a new value. The new type must match the type of this option.
    DtOpt&  operator =(int NewValue);
    DtOpt&  operator =(double NewValue);
    DtOpt&  operator =(bool NewValue);

    // Get the actual value out of this option.
    int  ToInt() const;
    bool  ToBool() const;
    double  ToDouble() const;
    wstring  ToString() const;

    // Change this string option to an integer option.
    void MakeInt(int Value);

    // Check if this option was used ont he commandline
    bool  IsSet() const { return m_IsSet; }

    // Parse this string option via the list of string->int pairs in EnumPairs and
    // make it an int.
    void  ParseEnum(const DtEnumOptPair* EnumPairs, wstring OptName);

private:
    bool  m_IsSet;                  // Whether or not this option has been used.
    DtOptType  m_Type;              // The type of this option
    union {
        int  m_IntValue;            // The int or boolean value that has been assigned
        double  m_DoubleValue;      // The double value that has been assigned
    };
    wstring  m_StrValue;            // The original value as specified on the commandline

    // DtOptItem is allowed to modify private members
    friend class DtOptItem;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- class DtOptItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
class DtOptItem
{
public:
    // Create a new DtOptItem that represens an integer option. If you specify EnumPairs
    // the actual parameter will be a string that is mapped according to that
    // string->int mapping. Otherwise an integer is expected on the commandline.
    DtOptItem(wstring Name, DtOpt& Option, int Default, wstring Desc, 
                                                   const DtEnumOptPair* EnumPairs = NULL);

    // Create a new DtOptItem that represens an integer option with given minimum and
    // maximum values.
    DtOptItem(wstring Name, DtOpt& Option, int Default, wstring Desc, int Min, int Max);
    
    // Create a new DtOptItem that represens a double option with given minimum and
    // maximum values.
    DtOptItem(wstring Name, DtOpt& Option, double Default, wstring Desc, double Min,
                                                                              double Max);

    // Create a new DtOptItem that represens a boolean option.
    DtOptItem(wstring Name, DtOpt& Option, bool Default, wstring Desc);

    // Create a new DtOptItem that represens a string option.
    DtOptItem(wstring Name, DtOpt& Option, wstring Desc);
    
    // Parse all options. Options points to an array of options, NumOpt contains the
    // total number of options. argc/argv are as in main. All arguments that are not
    // either an option or a parameter to an option will be returned in FreeArgs.
    static void  ParseOpt(DtOptItem* Options, int NumOpt, int argc, char* argv[], 
                                                                 list<wstring>& FreeArgs);
    
    // Template to autodetect the length of the Options array.
    template<int N>
    static void  ParseOpt(DtOptItem (&Options)[N], int argc, char* argv[], 
                                                                  list<wstring>& FreeArgs)
    {
        return ParseOpt(Options, N, argc, argv, FreeArgs);
    }

    // Write a description of given options to stdout.
    static void  PrintOptionsHelp(DtOptItem* Options, int NumOpt);

private:
    void  ParseIntOpt();
    void  ParseDoubleOpt();

    const DtOptType  m_Type;
    union {
        struct {
            int  m_IntValue;
            int  m_MinInt, m_MaxInt;
            const DtEnumOptPair* m_EnumPairs;
        };
        struct {
            double  m_DoubleValue;
            double  m_MinDouble, m_MaxDouble;
        };
    };

    const wstring m_Name;
    const wstring m_Description;
    DtOpt&  m_Option;
};

#endif
