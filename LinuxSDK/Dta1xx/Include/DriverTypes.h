//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DriverTypes.h *#*#*#*#*#*#*#*# (C) 2000-2007 DekTec
//
// Basic types used in Dta1xx device driver.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	SD	2000.10.08	Created

#ifndef __DRIVER_TYPES_H
#define __DRIVER_TYPES_H
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Basic Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef signed int				Int;
typedef signed char				Int8;
typedef signed short			Int16;
typedef signed int				Int32;
#ifdef __KERNEL__
	typedef __s64				Int64;
#else
	typedef signed long long	Int64;
#endif
typedef signed long				IntPtr;

typedef unsigned int			UInt;
typedef unsigned char			UInt8;
typedef unsigned short			UInt16;
typedef unsigned int			UInt32;
#ifdef __KERNEL__
	typedef __u64				UInt64;
#else
	typedef unsigned long long	UInt64;
#endif
typedef unsigned long			UIntPtr;

typedef UInt8					BOOLEAN;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#endif
