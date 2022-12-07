#ifndef __FROG__DEBUGC_H__
#define __FROG__DEBUGC_H__

// C-friendly debug functions go here.

#include "UtilityC.h"
#include "Platform.h"

/// Maximum number of characters allowed in a DebugPrintf call +1.
#define DEBUG_PRINT_BUFFER_LENGTH       2048

//=============================================================================

EXTERN_C_BEGIN

/// Print a single character to the debug output.
void DebugPutChar(const char _character);
/// Print to the debug output with the given pattern and a variable list of parameters.
void DebugPrintf(const char *_pattern, ...);
/// Print to the debug output with the given pattern and a variable list of parameters.
/// Add a newline at the end.
void DebugPrintLinef(const char *_pattern, ...);
/// Print to the debug output with the given pattern and argument list.
void DebugVPrintf(const char *_pattern, va_list arguments);

/// Print to the debug output with the given pattern and a variable list of
/// parameters.  This should work, even in release, depending on the platform.
void InfoPrintf(const char *_pattern, ...);
/// Print to the debug output with the given pattern and argument list.  This
/// should work, even in release, depending on the platform.
void InfoVPrintf(const char *_pattern, va_list arguments);

/// Print a message about a non-critical problem to the debug output with the
/// given pattern and a variable list of parameters.
void WarningPrintf(const char *_pattern, ...);
/// Print a message about a non-critical problem to the debug output with the
/// given pattern and argument list.
void WarningVPrintf(const char *_pattern, va_list arguments);

/// Print a message about a critical problem to the debug output with the
/// given pattern and a variable list of parameters.
void ErrorPrintf(const char *_pattern, ...);
/// Print a message about a critical problem to the debug output with the
/// given pattern and argument list.
void ErrorVPrintf(const char *_pattern, va_list arguments);

#ifdef _DEBUG
   /// Print the given message along with the current location in the sources to debug output.
   #define FrogTrace(message) DebugPrintf(message "\n\tin " __FILE__ "\n\tline " FROG_MACRO2STRING(__LINE__) "\n")
#else
   #define FrogTrace(message)
#endif

#if PLATFORM_IS_WINDOWS
   /// Begin a block in which crashes may be caught for debugging.
   #define FROG_DUMP_GUARD_BEGIN __try {
   /// End a block in which crashes may be caught for debugging.
   #define FROG_DUMP_GUARD_END } __except(FrogDumpGuardEndHelper((void*)GetExceptionInformation())) { }

   /// Called by FROG_DUMP_GUARD_END to help create dumps.
   int FrogDumpGuardEndHelper(void* _exceptionPointers);
#else
   #define FROG_DUMP_GUARD_BEGIN
   #define FROG_DUMP_GUARD_END
#endif

EXTERN_C_END

//=============================================================================

#endif //#ifndef __FROG__DEBUGC_H__
