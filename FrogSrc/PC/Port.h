#ifndef __FROG__PORT_H__
#define __FROG__PORT_H__

#include <algorithm>

// TryEnterCriticalSection requires Windows 2000 or better
#if (_WIN32_WINNT < 0x05000000)
   #undef _WIN32_WINNT
   #define _WIN32_WINNT 0x05000000
#endif

namespace Webfoot {

/// Signed 8-bit integer
typedef __int8 int8;
/// Signed 16-bit integer
typedef __int16 int16;
/// Signed 32-bit integer
typedef __int32 int32;
/// Signed 64-bit integer
typedef __int64 int64;
/// Unsigned 8-bit character
typedef unsigned __int8 uchar;
/// Unsigned 8-bit integer
typedef unsigned __int8 uint8;
/// Unsigned 16-bit integer
typedef unsigned __int16 uint16;
/// Unsigned 32-bit integer
typedef unsigned __int32 uint32;
/// Unsigned 64-bit integer
typedef unsigned __int64 uint64;
/// 32-bit floating point number
typedef float float32;
/// 64-bit floating point number
typedef double float64;

/// Integer form of true
#define TRUE 1
/// Integer form of false
#define FALSE 0

/// The PC is little-endian
#define FROG_BIG_ENDIAN 0

// Used for packing structures 
#define __GCC_PACKED__
#define __PRAGMA_PACK__ 1

} //namespace Webfoot {

#endif //#ifndef __FROG__PORT_H__
