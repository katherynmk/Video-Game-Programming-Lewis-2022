#ifndef __FROG__DUCK__ENVIRONMENTMAP_H__
#define __FROG__DUCK__ENVIRONMENTMAP_H__

#include "FrogMemory.h"

namespace Webfoot {

class Bitmap;

namespace Duck {

//==============================================================================

/// Base class for a map of the environment, like a cube map.
class EnvironmentMap
{
public:
   virtual void Init(Bitmap* _bitmap) { (void)_bitmap; mipmapLevelCount = 1; }
   virtual void Deinit() { mipmapLevelCount = 1; }

   /// Return the number of mipmap levels
   int MipmapLevelCountGet() { return mipmapLevelCount; }

protected:
   /// Number of mipmap levels.
   int mipmapLevelCount;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__ENVIRONMENTMAP_H__
