#ifndef __FROG__FROGSTRING_H__
#define __FROG__FROGSTRING_H__

#include "FrogMemory.h"
#include <stdarg.h>
#include <string.h>

namespace Webfoot {

//==============================================================================

/// Warning: This does not behave exactly like the normal snprintf.
/// Do a formatted print to 'buffer' without exceeding the size of the buffer.
/// 'bufferSize' is the size of 'buffer' in bytes, including the space for the
/// null terminator.  As long as 'bufferSize' is at least 1, 'buffer' will be
/// null-terminated.  Return false if truncation or some other problem occurs.
bool FrogSnprintf(char* buffer, size_t bufferSize, const char* format, ...);

/// Warning: This does not behave exactly like the normal vsnprintf.
/// Do a formatted print to 'buffer' without exceeding the size of the buffer.
/// 'bufferSize' is the size of 'buffer' in bytes, including the space for the
/// null terminator.  As long as 'bufferSize' is at least 1, 'buffer' will be
/// null-terminated.  Return false if truncation or some other problem occurs.
bool FrogVsnprintf(char* buffer, size_t bufferSize, const char* format, va_list args);

/// Warning: This does not behave exactly like the normal strncpy.
/// Copy from 'source' to 'destination' without exceeding the size of the
/// destination.  'destinationSize' is the size of the destination in bytes,
/// including the space for the null terminator.  As long as 'destinationSize'
/// is at least 1, 'destination' will be null-terminated.  Depending on the
/// platform, the end of 'destination' may or may not be padded with zeros.
/// If 'source' and 'destination' overlap, the behavior is undefined.  
/// Return false if truncation or some other problem occurs.
bool FrogStrncpy(char* destination, const char* source, size_t destinationSize);

/// Warning: This does not behave exactly like the normal strncat.
/// Append 'source' to 'destination' without exceeding the size of the
/// destination.  'destinationSize' is the size of the destination in bytes,
/// including the space for the null terminator.  As long as 'destinationSize'
/// is at least 1, 'destination' will be null-terminated.  If 'source' and
/// 'destination' overlap, the behavior is undefined.  Return false if
/// truncation or some other problem occurs.
bool FrogStrncat(char* destination, const char* source, size_t destinationSize);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FROGSTRING_H__
