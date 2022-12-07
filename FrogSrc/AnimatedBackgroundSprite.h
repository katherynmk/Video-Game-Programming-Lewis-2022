#ifndef __FROG__ANIMATEDBACKGROUNDSPRITE_H__
#define __FROG__ANIMATEDBACKGROUNDSPRITE_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "AnimatedBackgroundItem.h"
#include "SplineLinear.h"

namespace Webfoot {

class Sprite;
class AnimatedBackground;
class JSONValue;

//=============================================================================

class AnimatedBackgroundSprite : public AnimatedBackgroundItem
{
public:
   AnimatedBackgroundSprite();
   virtual ~AnimatedBackgroundSprite() {}

   /// Load the background item from the given JSONValue
   void Init(JSONValue* specifications, AnimatedBackground* _background);
   /// Set all relevant members with a single constructor.
   void Init(const char* _spriteName, int _startTime, unsigned int _fadeDuration, unsigned int _opaqueDuration,
      const Point2F& _startPosition, const Point2F& _velocity, AnimatedBackground* _background);
   /// Release the sprite
   virtual void Deinit();

   /// Draw the item.  Pass the current time in the AnimatedBackground in milliseconds.
   virtual void Draw(unsigned int currentTime);

   /// Use the sprite animation with the given name from the given file.
   /// If no sprite file is specified, it will assume it is in the background's
   /// sprite file.
   virtual void AnimationSet(const char* resourceFilename, const char* animationName);

protected:
   /// Cache any useful information based on the current values of the members.
   void Precompute();

   /// The sprite to be displayed
   Sprite* sprite;
   /// How far into the animation before the fade-in starts.
   int startTime;
   /// The length of a fade in milliseconds
   unsigned int fadeDuration;
   /// The amount of time for a given sprite to be shown at full opacity before fading out
   unsigned int opaqueDuration;
   /// The position of the sprite at the beginning of the fade-in.
   Point2F startPosition;
   /// The velocity of the sprite in pixels per millisecond
   Point2F velocity;
   /// Spline for controlling the position.
   SplineLinear<Point2F>* positionSpline;
   /// Spline for controlling the rotation.
   SplineLinear<float>* rotationSpline;
   /// Spline for controlling the scale.
   SplineLinear<Point2F>* scaleSpline;
   /// Spline for controlling the color.
   SplineLinear<ColorRGBA8>* colorSpline;

   /// Cached time at which the fade-in ends
   unsigned int fadeInEndTime;
   /// Cached time at which the opaque time ends
   unsigned int opaqueEndTime;
   /// Cached time at which the fade-out ends
   unsigned int fadeOutEndTime;

   typedef AnimatedBackgroundItem Inherited;
};

//=============================================================================

} // namespace Webfoot

#endif //#ifndef __FROG__ANIMATEDBACKGROUNDSPRITE_H__
