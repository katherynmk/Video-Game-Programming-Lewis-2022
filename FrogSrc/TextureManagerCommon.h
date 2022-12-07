#ifndef __FROG__TEXTUREMANAGERCOMMON_H__
#define __FROG__TEXTUREMANAGERCOMMON_H__

#include "FrogMemory.h"
#include "Texture.h"
#include "FileManager.h"
#include "FileReferenceCounter.h"

namespace Webfoot {

//==============================================================================

/// TextureManagerCommon contains the functionality that will be common to all
/// TextureManager implementations. 
class TextureManagerCommon
{
public:
   enum Options
   {
      /// Do not explicitly premultiply the alpha.  This is useful when loading
      /// a texture in which the channels are not used for color and alpha.
      SKIP_PREMULTIPLY_ALPHA = 1,
      /// Generate mipmaps automatically.  This is not necessarily supported on
      /// all platforms, all formats, or all texture sizes.
      GENERATE_MIPMAPS = 2
   };

   virtual ~TextureManagerCommon() {}

   void Init(HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit();

   /// Free the given texture.
   void Unload(const char* filename, FileManager* fileManager = theFiles);
   /// Free the given texture.
   void Unload(Texture* texture);

protected:
   /// Collection of textures
   FileReferenceCounter textures;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TEXTUREMANAGERCOMMON_H__
