#ifndef __WAGESPRITE_H__
#define __WAGESPRITE_H__

#include "Frog.h"
#include "WageDrawable.h"
#include "WageShadowFD.h"

#define MAX_Z_DIFFERENCE_DEFAULT 10.0f
#define SKILL_COUNT 8

namespace Webfoot {

//==============================================================================

class WageSprite : public Sprite, public WageDrawable
{
public:
   typedef Sprite Inherited;

   WageSprite();
   virtual ~WageSprite() {}
   
   virtual void Init(const char* resourceFilename, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void Draw();

   /// If the autoupdate flag is set, the engine will update the sprite
   /// If the flag is false, the programmer must update the sprite manually (for varying loop durations)
   void AutoUpdateSet(bool _autoUpdate) { autoUpdate = _autoUpdate; }
   /// Sets the auto update flag
   bool AutoUpdateCheck() { return autoUpdate; }
   /// Get the name of the wage sprite
   const char* NameGet() { return name; }
   /// Get the number of the sprite
   int NumberGet() { return number; }
   /// Get the skill at the index specified
   int SkillGet(int index) { if(index < SKILL_COUNT) return skills[index]; else return 0; }
   /// Set the skill at the index specified
   void SkillSet(int index, int newValue) { if(index < SKILL_COUNT) skills[index] = newValue; else return; }
   /// Set the transparency of the sprite (modifies the sprite color attribute)
   void TransparencySet(float _transparency);
   /// Get the transparency of the sprite
   float TransparencyGet() { return transparency; }
   /// Set the invertX flag
   void InvertXSet(bool _invertX) { invertX = _invertX; }
   /// Get the invertX flag
   bool InvertXCheck() { return invertX; }
   /// Set the invertY flag
   void InvertYSet(bool _invertY) { invertY = _invertY; }
   /// Get the invertY flag
   bool InvertYCheck() { return invertY; }
   /// Sets the width of the sprite in world units
   void WidthSet(float _width) { widthPercent = 0.0f; width = _width; AnimationOffsetSet(); }
   /// Gets the width of the sprite in world units
   float WidthGet() 
   {
      if(widthPercent > 0.0f)
         return widthPercent * FullBoundsGet().width;
      else
         return width;
   }
   /// Sets the height of the sprite in world units
   void HeightSet(float _height) { heightPercent = 0.0f; height = _height; AnimationOffsetSet(); }
   /// Gets the height of the sprite in world units
   float HeightGet()
   {
      if(heightPercent > 0.0f)
         return heightPercent * FullBoundsGet().height;
      else
         return height;
   }
   /// Gets the shadow sprite (if applicable)
   WageShadow* ShadowGet() { return shadow; }
   /// Check if this object is marked for deletion by the wage engine
   bool MarkForDeleteCheck() { return markForDelete; }
   /// Mark this object for deletion by the wage engine
   void MarkForDeleteSet() { VisibleSet(false); CanCollideSet(false); markForDelete = true; }
   /// Sets the animation using the provided resourceFilename and animationName
   virtual void AnimationSet(const char* resourceFilename, const char* animationName);
   /// Sets the animation using the given animation and the currently loaded sprite resource
   virtual void AnimationSet(const char* animationName);
   /// Gets the currently set animation offset in 3d space
   Point2F AnimationOffsetGet();
   /// Sets up the animation offset for 3d space based on the sprite dimensions
   virtual void AnimationOffsetSet();
   /// Set the animation offset for 3d space
   void AnimationOffsetSet(const Point2F& offset);
   /// Returns true if the other sprite is colliding with this sprite
   bool CollisionCheck(WageSprite* otherSprite);
   /// Returns true if the cube is colliding with this sprite
   bool CollisionCheck(WageCube* otherCube);
   /// Returns the collision box associated with the sprite
   Box3F CollisionBoxGet() { return collisionBox + Position3Get(); }
   /// Set to true if the sprite should be drawn
   void CanCollideSet(bool _canCollide) { canCollide = _canCollide; }
   /// Return true if the sprite should be drawn.
   virtual bool CanCollideCheck() { return canCollide; }

   /// Return the depth component of the sprite's position.
   virtual float DepthGet() { return Inherited::DepthGet(); }

protected:
   /// Auto update flag (whether or not the engine should update this sprite)
   bool autoUpdate;
   /// If true, the sprite will be considered for collision check (if also visible)
   bool canCollide;
   /// Mark for deletion/garbage collection
   bool markForDelete;
   /// Name of the wage sprite
   const char* name;
   /// Sprite number
   int number;
   /// Skills table for game-specific functionality
   int skills[SKILL_COUNT];
   /// Texture width in world units
   float width;
   /// Texture width as a percent of sprite width
   float widthPercent;
   /// Texture height in world units
   float height;
   /// Texture height as a percent of sprite height
   float heightPercent;
   /// Transparency (decimal between 0-1) to apply to the sprite
   float transparency;
   /// 0-16 integer to denote light intensity
   int intensity;
   /// If true, invert the sprite horizontally
   bool invertX;
   /// If true, invert the sprite vertically
   bool invertY;
   /// Collision box
   Box3F collisionBox;
   /// Shadow sprite
   WageShadow* shadow;
   /// Current animation according to the offset... 
   /// If this changes, the update function will automatically call AnimationOffsetSet()
   SpriteAnimation* currentAnimation;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGESPRITE_H__
