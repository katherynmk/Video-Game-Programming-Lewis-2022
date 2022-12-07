#ifndef __FROG__TEXTURECOMMON_H__
#define __FROG__TEXTURECOMMON_H__

#include "FrogMemory.h"
#include "Point2.h"

namespace Webfoot {

class Bitmap;

//=============================================================================

/// TextureCommon contains functionality and data common to all the
/// platform-specific implementations of Texture.  It is not primarily an interface.
/// All Textures should ultimately be created by theScreen->TextureCreate.
class TextureCommon
{
public:
   enum
   {
      /// Generate mipmaps automatically.  This is not necessarily supported on
      /// all platforms, all formats, or all texture sizes.
      GENERATE_MIPMAPS = 1
   };

   TextureCommon();
   
   /// Return the current bitmap for the largest mipmap level.  If the bitmap
   /// was not set with BitmapSet, this may return NULL.
   Bitmap* BitmapGet() { return bitmap; }

   /// Return the width of the texture in texels at mipmap level 0.
   int WidthGet();
   /// Return the height of the texture in texels at mipmap level 0.
   int HeightGet();
   /// Return the dimensions of the texture in texels at mipmap level 0.
   Point2I SizeGet();

   /// Return true if this texture includes opacity information.
   bool AlphaCheck();

   /// Return the number of mipmap levels
   int MipmapLevelCountGet() { return mipmapLevelCount; }

   /// If this is a running on a platform that does not need to keep the bitmap
   /// data in main memory for drawing (OpenGL, for example),
   /// free the bitmap data for this Texture.  This will not free
   /// the objects for the texture or bitmap objects, only the pixel buffer.
   void UnnecessaryBitmapDataDeallocate();

protected:
   /// Number of mipmap levels.
   int mipmapLevelCount;
   /// Bitmap associated with the largest mipmap level
   Bitmap* bitmap;
};

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TEXTURECOMMON_H__
