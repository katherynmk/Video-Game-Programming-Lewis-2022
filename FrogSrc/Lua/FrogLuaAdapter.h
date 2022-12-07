#ifndef __FROG__FROGLUAADAPTER_H__
#define __FROG__FROGLUAADAPTER_H__

#include "UtilityC.h"

//=============================================================================

EXTERN_C_BEGIN

struct lua_State;

/// Debug print function to be called from Lua.
void FrogLuaPrintF(const char* format, ...);
/// Debug warning function to be called from Lua.
void FrogLuaWarningF(const char* format, ...);

/// Frog-specific implementation of feof
int FrogLuaFeof(void* _file);
/// Frog-specific implementation of fread
size_t FrogLuaFread(void* buffer, size_t size, size_t count, void* _file);
/// Frog-specific implementation of fopen.  This currently assumes you'll be reading.
void* FrogLuaFopen(lua_State *L, const char* filename, const char* mode);
/// Frog-specific implementation of fclose.
void FrogLuaFclose(lua_State *L, void* _file);

EXTERN_C_END

//=============================================================================

#endif //#ifndef __FROG__FROGLUAADAPTER_H__
