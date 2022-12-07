#ifndef __WAGESHADOWCOMMON_H__
#define __WAGESHADOWCOMMON_H__

#include "Frog.h"
#include "WageSprite.h"

/// Maximum number of shadow splits
#define MAX_SPLIT 12
/// Vertexes per shadow split
#define VERTEX_COUNT 4

namespace Webfoot {

class WageSprite;

//==============================================================================

class WageShadowCommon
{
public:
   WageShadowCommon() {}
   ~WageShadowCommon() {}
   
   virtual void Init(WageSprite* _parentSprite);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Set the offset of the shadow from the parent sprite position
   void OffsetSet(const Point3F& _offset) { offset = _offset; }
   /// Get the offset of the shadow from the parent sprite position
   Point3F OffsetGet() { return offset; }
   /// Add to the offset
   void OffsetAdd(const Point3F& _offset) { offset += _offset; }
   /// Set the width of the shadow in world units
   void WidthSet(float _width) { width = _width; }
   /// Get the width of the shadow in world units
   float WidthGet() { return width; }
   /// Set the height of the shadow in world units
   void HeightSet(float _height) { height = _height; }
   /// Get the height of the shadow in world units
   float HeightGet() { return height; }
   /// Lock to sprite check
   bool LockToSpriteCheck() { return lockToSprite; }
   /// Locks the shadow to be at or below the sprite's height
   /// This is turned off when the shadow is likely to be cast on an incline 
   /// because half of the shadow is going to be above the sprite's position (see rocks in CaveManRocks)
   void LockToSpriteSet(bool _lockToSprite) { lockToSprite = _lockToSprite; }
   /// Returns true if shadow is visible
   bool VisibleCheck() { return visible; }

   struct ShadowParts
   {
      /// Normalized texture coordinates
      float textureMinX;
      float textureMinZ;
      float textureMaxX;
      float textureMaxZ;

      /// Destination region of the shadow in world units
      Point3F vertices[VERTEX_COUNT];
   };

protected:
   /// The shadow texture
   Texture* texture;
   /// The sprite being shadowed
   WageSprite* parentSprite;
   /// Number of splits for the current draw
   int splitCount;
   /// Source and destination rectangles of the shadow (when not split the count = 1)
   ShadowParts parts[MAX_SPLIT];
   /// Offset from the parent position
   Point3F offset;
   /// Width of the texture in world units
   float width;
   /// Height of the texture in world units
   float height;
   /// Used for alpha tweaking (with height factor)
   ColorRGBA8 color;
   /// Lock shadow to sprite (as opposed to projecting on the cube, regardless of player position)
   bool lockToSprite;
   /// True if the shadow is visible
   bool visible;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGESHADOWCOMMON_H__
