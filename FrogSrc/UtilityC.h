#ifndef __FROG__UTILITYC_H__
#define __FROG__UTILITYC_H__

#include <stdarg.h>
#include <stddef.h>

// Platform-independed C-friendly utilities go here.

/// Helper macro to FROG_MACRO2STRING
#define FROG_MACRO2STRING2(x) #x
/// Convert the given number to a string for macro purposes.
#define FROG_MACRO2STRING(x) FROG_MACRO2STRING2(x)

#ifdef __cplusplus
  /// Begin an 'extern "C"' block if used from C++.
  #define EXTERN_C_BEGIN extern "C" {
  /// Close an 'extern "C"' block if used from C++.
  #define EXTERN_C_END              }
#else
   /// Begin an 'extern "C"' block if used from C++.
  #define EXTERN_C_BEGIN
  /// Close an 'extern "C"' block if used from C++.
  #define EXTERN_C_END
#endif

EXTERN_C_BEGIN

/// C-friendly version of UTF8Snprintf.  Returns 1 for success and 0 for failure.
int FrogUTF8Snprintf(char* buffer, size_t bufferSize, const char* format, ...);
/// C-friendly version of UTF8Vsnprintf.  Returns 1 for success and 0 for failure.
int FrogUTF8Vsnprintf(char* buffer, size_t bufferSize, const char* format, va_list args);
/// C-friendly version of UTF8Strncpy.  Returns 1 for success and 0 for failure.
int FrogUTF8Strncpy(char* destination, const char* source, size_t destinationSize);
/// C-friendly version of UTF8Strncat.  Returns 1 for success and 0 for failure.
int FrogUTF8Strncat(char* destination, const char* source, size_t destinationSize);

EXTERN_C_END

#endif //#ifndef __FROG__UTILITYC_H__
