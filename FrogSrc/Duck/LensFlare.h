#ifndef __FROG__DUCK__LENSFLARE_H__
#define __FROG__DUCK__LENSFLARE_H__

#include "FrogMemory.h"
#include "Point2.h"

namespace Webfoot {

class JSONValue;
class Sprite;

namespace Duck {

class LensFlareSprite;
class SceneNodeLight;

/// True if a ray test should be used to determine whether a light is occluded.
#define DUCK_LENS_FLARE_OCCLUSION_RAY_TEST_ENABLED_KEY "OcclusionRayTestEnabled"
/// Maximum distance to use in occlusion ray tests for directional lights.
#define DUCK_LENS_FLARE_OCCLUSION_RAY_TEST_DIRECTIONAL_LIGHT_DISTANCE_KEY "OcclusionRayTestDirectionalLightDistance"
/// Sprites which make up the lens flare.
#define DUCK_LENS_FLARE_SPRITES_KEY "Sprites"
/// File with the definition of the sprite animation.
#define DUCK_LENS_FLARE_SPRITE_SPRITE_RESOURCE_FILENAME_KEY "SpriteResourceFilename"
/// Which sprite animation to use from the specified resource file.
#define DUCK_LENS_FLARE_SPRITE_ANIMATION_KEY "Animation"
/// Multiplied by the projected position of the light on the screen, relative
/// to the center of the screen, to determine the position of this element.
#define DUCK_LENS_FLARE_SPRITE_POSITION_COEFFICIENT_KEY "PositionCoefficient"
/// Rotation applied to the sprite, in degrees.
#define DUCK_LENS_FLARE_SPRITE_ROTATION_KEY "Rotation"
/// Scale applied to the sprite.
#define DUCK_LENS_FLARE_SPRITE_SCALE_KEY "Scale"
/// Color applied to the sprite.
#define DUCK_LENS_FLARE_SPRITE_COLOR_KEY "Color"

//==============================================================================

/// An object for managing a lens flare.
class LensFlare
{
public:
   void Init(JSONValue* specs);
   void Deinit();
   void Update(unsigned int dt);
   void Draw(SceneNodeLight* light);

protected:
   /// True if a ray test should be used to determine whether a light is occluded.
   bool occlusionRayTestEnabled;
   /// Maximum distance to use in occlusion ray tests for directional lights.
   float occlusionRayTestDirectionalLightDistance;
   /// Collection of sprites that make up the effect.
   Table<LensFlareSprite*> lensFlareSprites;
};

//==============================================================================

/// An object for a single element of a sprite-based lens flare.
class LensFlareSprite
{
public:
   LensFlareSprite();

   void Init(JSONValue* specs);
   void Deinit();
   void Update(unsigned int dt);
   void Draw();

   /// Update the position of this element given that the light producing the
   /// lens flare is at the given position relative to the center of the
   /// screen.
   void PositionRefresh(const Point2F& lightOffsetFromCenter);

protected:
   /// Sprite used to draw this element of the lens flare.
   Sprite* sprite;
   /// Multiplied by the projected position of the light on the screen,
   /// relative to the center of the screen, to determine the position of this
   /// element.
   float positionCoefficient;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__LENSFLARE_H__
