#ifndef __FROG__TEXTUREMANAGER_H__
#define __FROG__TEXTUREMANAGER_H__

#include "FrogMemory.h"

namespace Webfoot {

/// Select the implementation of TextureManager to use here.
class TextureManagerOpenGL;
typedef TextureManagerOpenGL TextureManager;

} //namespace Webfoot {

#include "TextureManagerOpenGL.h" 

#endif //#ifndef __FROG__TEXTUREMANAGER_H__
