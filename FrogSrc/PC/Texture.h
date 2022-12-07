#ifndef __FROG__TEXTURE_H__
#define __FROG__TEXTURE_H__

#include "FrogMemory.h"
#include "TextureOpenGL.h"

namespace Webfoot {
   class TextureOpenGL;
   typedef TextureOpenGL Texture;
   
   /// Minimum length of the edge of a texture.  Must be a power of 2.
   const int TEXTURE_EDGE_MIN = 8;
   /// Maximum length of the edge of a texture.  Must be a power of 2.
   const int TEXTURE_EDGE_MAX = 2048;
} //namespace Webfoot {

#endif //#ifndef __FROG__TEXTURE_H__
