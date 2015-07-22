//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtOpt.cpp *#*#*#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// DtOpt - DekTec command-line option interpreter - Implementation

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtOpt.h"
#include <algorithm>
#include <cassert>
#include <limits.h>
#include <sstream>
#include <wchar.h>

#ifndef WINBUILD
#define _vsnwprintf vswprintf
#endif

static int wtoi(wstring Str)
{
    long  Result;
    if (Str.size() > 2 && Str.substr(0, 2) == L"0x")
    {
        Result = wcstol(Str.c_str() + 2, NULL, 16);
    } else {
        Result = wcstol(Str.c_str(), NULL, 10);
    }
    return (int)Result;
}

static double wtof(wstring Str)
{
    wistringstream Stream(Str);
    double  Result;
    Stream >> Result;
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOptException::DtOptException -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtOptException::DtOptException(const wchar_t* pFormat, ...)
{
    va_list  ArgList;
    va_start(ArgList, pFormat);
    _vsnwprintf(m_ErrorMsg, sizeof(m_ErrorMsg)-1, pFormat, ArgList);
    va_end(ArgList);
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOpt::DtOpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtOpt::DtOpt() :
    m_IsSet(false),
    m_Type(OPT_TYPE_INVALID)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOpt::operator int -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Convert DtOpt object back to an int
//
DtOpt::operator int() const
{
    assert(m_Type==OPT_TYPE_INT || m_Type==OPT_TYPE_BOOL);
    return m_IntValue;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOpt::operator = -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Assign a new value to the DtOpt object
//
DtOpt&  DtOpt::operator =(int  NewValue)
{
    assert(m_Type == OPT_TYPE_INT);
    m_IntValue = NewValue;
    return *this;
}
DtOpt&  DtOpt::operator =(double  NewValue)
{
    assert(m_Type == OPT_TYPE_DOUBLE);
    m_DoubleValue = NewValue;
    return *this;
}
DtOpt&  DtOpt::operator =(bool  NewValue)
{
    assert(m_Type == OPT_TYPE_BOOL);
    m_IntValue = NewValue;
    return *this;
}
DtOpt&  DtOpt::operator =(long long  NewValue)
{
    assert(m_Type == OPT_TYPE_INT64);
    m_Int64Value = NewValue;
    return *this;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOpt::To* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Explicitely convert DtOpt to the underlying base type
//
int  DtOpt::ToInt() const
{
    assert(m_Type == OPT_TYPE_INT);
    return m_IntValue;
}
long long  DtOpt::ToInt64() const
{
    assert(m_Type == OPT_TYPE_INT64);
    return m_Int64Value;
}
bool  DtOpt::ToBool() const
{
    assert(m_Type == OPT_TYPE_BOOL);
    return m_IntValue != 0;
}
double  DtOpt::ToDouble() const
{
    assert(m_Type == OPT_TYPE_DOUBLE);
    return m_DoubleValue;
}
wstring  DtOpt::ToString() const
{
    return m_StrValue;
}
wstring  DtOpt::ToLower() const
{
    wstring  Str = m_StrValue;
    transform(Str.begin(), Str.end(), Str.begin(), ::tolower);
    return Str;
}
list<wstring>  DtOpt::GetStringList() const
{
    assert(m_Type == OPT_TYPE_STRING_LIST);
    return m_Strings;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOpt::MakeInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtOpt::MakeInt(int Value)
{
    m_Type = OPT_TYPE_INT;
    m_IntValue = Value;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOpt::ParseEnum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtOpt::ParseEnum(const DtEnumOptPair* EnumPairs, wstring OptName)
{
    while (EnumPairs->m_Name != L"")
    {
        if (EnumPairs->m_Name == m_StrValue)
        {
            MakeInt(EnumPairs->m_Value);
            return;
        }
        EnumPairs++;
    }
    throw DtOptException(L"Invalid argument for command line option: -%ls",
                                                                         OptName.c_str());
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOptItem::DtOptItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtOptItem::DtOptItem(wstring Name, DtOpt& Option, int Default, wstring Desc, 
                                                         const DtEnumOptPair* EnumPairs) :
    m_Type(OPT_TYPE_INT),
    m_IntValue(Default),
    m_MinInt(INT_MIN),
    m_MaxInt(INT_MAX),
    m_EnumPairs(EnumPairs),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, int Default, wstring Desc, 
                                                                       int Min, int Max) :
    m_Type(OPT_TYPE_INT),
    m_IntValue(Default),
    m_MinInt(Min),
    m_MaxInt(Max),
    m_EnumPairs(NULL),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, long long  Default, wstring Desc) :
    m_Type(OPT_TYPE_INT64),
    m_Int64Value(Default),
    m_MinInt64(LLONG_MIN),
    m_MaxInt64(LLONG_MAX),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, long long  Default, wstring Desc,
                                                           long long Min, long long Max) :
    m_Type(OPT_TYPE_INT64),
    m_Int64Value(Default),
    m_MinInt64(Min),
    m_MaxInt64(Max),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, double Default, wstring Desc, 
                                                                 double Min, double Max) :
    m_Type(OPT_TYPE_DOUBLE),
    m_DoubleValue(Default),
    m_MinDouble(Min),
    m_MaxDouble(Max),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, bool Default, wstring Desc) :
    m_Type(OPT_TYPE_BOOL),
    m_IntValue(Default),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, wstring Desc) :
    m_Type(OPT_TYPE_STRING),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

DtOptItem::DtOptItem(wstring Name, DtOpt& Option, wstring Desc, int NumArgs) :
    m_Type(OPT_TYPE_STRING_LIST),
    m_IntValue(NumArgs),
    m_Name(Name),
    m_Description(Desc),
    m_Option(Option)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOptItem::ParseDoubleOpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtOptItem::ParseDoubleOpt()
{
    m_Option.m_DoubleValue = wtof(m_Option.m_StrValue.c_str());
    if (m_Option.m_DoubleValue<m_MinDouble || m_Option.m_DoubleValue>m_MaxDouble)
        throw DtOptException(L"Invalid argument for command line option: -%ls", 
                                                                          m_Name.c_str());
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOptItem::ParseIntOpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtOptItem::ParseIntOpt()
{
    if (m_EnumPairs != NULL)
    {
        m_Option.ParseEnum(m_EnumPairs, m_Name);
    } else {
        m_Option.m_IntValue = wtoi(m_Option.m_StrValue.c_str());
        if (m_Option.m_IntValue<m_MinInt || m_Option.m_IntValue>m_MaxInt)
            throw DtOptException(L"Invalid argument for command line option: -%ls", 
                                                                          m_Name.c_str());
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOptItem::ParseInt64Opt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtOptItem::ParseInt64Opt()
{
    assert(m_EnumPairs != NULL);

    if (swscanf(m_Option.m_StrValue.c_str(), L"%lld", &m_Option.m_Int64Value) != 1)
        throw DtOptException(L"Failed to parse 64-bit integer argument: %ls",
                                                             m_Option.m_StrValue.c_str());
    if (m_Option.m_Int64Value<m_MinInt64 || m_Option.m_Int64Value>m_MaxInt64)
        throw DtOptException(L"Invalid argument for command line option: -%ls", 
                                                                          m_Name.c_str());
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtOptItem::ParseOpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtOptItem::ParseOpt(DtOptItem* Options, int NumOpt, int argc, char* argv[], 
                                                                  list<wstring>& FreeArgs)
{
    // First assign default values to all options
    for (int i=0; i<NumOpt; i++)
    {
        Options[i].m_Option.m_Type = Options[i].m_Type;
        switch (Options[i].m_Type)
        {
        case OPT_TYPE_BOOL:
        case OPT_TYPE_INT:
            Options[i].m_Option.m_IntValue = Options[i].m_IntValue;
            break;
        case OPT_TYPE_INT64:
            Options[i].m_Option.m_Int64Value = Options[i].m_Int64Value;
            break;
        case OPT_TYPE_DOUBLE:
            Options[i].m_Option.m_DoubleValue = Options[i].m_DoubleValue;
            break;
        case OPT_TYPE_STRING:
            break;
        case OPT_TYPE_STRING_LIST:
            Options[i].m_Option.m_Strings.clear();
            Options[i].m_Option.m_IntValue = Options[i].m_IntValue;
            for (int j=0; j<Options[i].m_Option.m_IntValue; j++)
                Options[i].m_Option.m_Strings.push_back(wstring());
            break;
        default:
            assert(false);
        }
    }
    bool  CheckOptions = true;
    // And then parse the actual commandline arguments
    for (int i=1; i<argc; i++)
    {
#ifdef WINBUILD
        if (CheckOptions && (argv[i][0]=='-' || argv[i][0]=='/'))
#else
        if (CheckOptions && argv[i][0]=='-')
#endif
        {
            if (argv[i][1] == '-')
            {
                CheckOptions = false;
                continue;
            }
            string  OptName(argv[i] + 1);
            wstring  WOptName(OptName.begin(), OptName.end());
            int j;
            for (j=0; j<NumOpt; j++)
            {
                if (Options[j].m_Name != WOptName)
                    continue;

                Options[j].m_Option.m_IsSet = true;
                if (Options[j].m_Type == OPT_TYPE_BOOL)
                {
                    Options[j].m_Option.m_IntValue = true;
                    break;
                }

                if (Options[j].m_Type == OPT_TYPE_STRING_LIST)
                {
                    if (i == argc - Options[j].m_IntValue)
                        throw DtOptException(L"Not enough arguments for command-line "
                                                            L"option: -%ls (expected %d)",
                                                            WOptName.c_str(),
                                                            Options[j].m_IntValue);
                    list<wstring>::iterator  It=Options[j].m_Option.m_Strings.begin();
                    for (int k=0; k<Options[j].m_IntValue; k++)
                    {
                        string  OptArg(argv[++i]);
                        *It = wstring(OptArg.begin(), OptArg.end());
                        It++;
                    }
                    break;
                }

                if (i == argc - 1)
                    throw DtOptException(L"Missing argument for command-line option: -%ls", 
                                                                        WOptName.c_str());
                string  OptArg(argv[++i]);
                Options[j].m_Option.m_StrValue = wstring(OptArg.begin(), OptArg.end());

                if (Options[j].m_Type == OPT_TYPE_DOUBLE)
                    Options[j].ParseDoubleOpt();
                else if (Options[j].m_Type == OPT_TYPE_INT)
                    Options[j].ParseIntOpt();
                else if (Options[j].m_Type == OPT_TYPE_INT64)
                    Options[j].ParseInt64Opt();
                break;
            }
            if (j == NumOpt)
                throw DtOptException(L"Unknown command-line option (-%ls) encountered", 
                                                                        WOptName.c_str());
        } else {
            string  Opt(argv[i]);
            FreeArgs.push_back(wstring(Opt.begin(), Opt.end()));
        }
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtOptItem::PrintOptionsHelp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtOptItem::PrintOptionsHelp(DtOptItem* Options, int NumOpt)
{
    for (int i=0; i<NumOpt; i++)
    {
        wprintf(L"   -%-10ls", Options[i].m_Name.c_str());
        size_t  Pos = Options[i].m_Description.find(L'\n');
        wprintf(L"%ls\n", Options[i].m_Description.substr(0, Pos).c_str());
        while (Pos != wstring::npos)
        {
            size_t  LastPos = Pos + 1;
            Pos = Options[i].m_Description.find(L'\n', LastPos);
            wprintf(L"         %ls\n",
                         Options[i].m_Description.substr(LastPos, Pos - LastPos).c_str());
        }
        if (Options[i].m_Type==OPT_TYPE_INT && Options[i].m_EnumPairs!=NULL)
        {
            const DtEnumOptPair*  OptPair = Options[i].m_EnumPairs;
            while (OptPair->m_Name != L"")
            {
                wprintf(L"           %-9ls %ls\n", OptPair->m_Name.c_str(), 
                                                          OptPair->m_Description.c_str());
                OptPair++;
            }
        }
        wprintf(L"\n");
    }
}
