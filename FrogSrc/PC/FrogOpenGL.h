#ifndef __FROG__FROGOPENGL_H__
#define __FROG__FROGOPENGL_H__

#include "FrogMemory.h"

#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/wglew.h"

/// 1 if using OpenGL ES and 0 otherwise.
#define FROG_OPENGL_ES 0

/// Type used by Screen to identify display modes on this platform.
typedef DWORD PlatformDisplayMode;

#endif //#ifndef __FROG__FROGOPENGL_H__
