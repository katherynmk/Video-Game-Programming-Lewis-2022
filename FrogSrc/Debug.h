#ifndef __FROG__DEBUG_H__
#define __FROG__DEBUG_H__

#include "FrogMemory.h"
#include <assert.h>
#include <stdarg.h>
#include "DebugC.h"

namespace Webfoot {

/// Filename to use by default for the debug log.
#define DEBUG_LOG_FILENAME_DEFAULT "Log.txt"

//===============================================================================

/// Initialize the debug system.
void DebugInit(const char* logFilename = DEBUG_LOG_FILENAME_DEFAULT);
/// Initialize the debug system based on the given command line arguments.
void DebugInit(int argc, const char** argv);

/// Deinitialize the debug system.
void DebugDeinit();

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__DEBUG_H__
