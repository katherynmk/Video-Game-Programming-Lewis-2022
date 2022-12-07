#ifndef __FROG__ANIMATEDBACKGROUNDITEM_H__
#define __FROG__ANIMATEDBACKGROUNDITEM_H__

#include "FrogMemory.h"

namespace Webfoot {

class JSONValue;
class AnimatedBackground;

//=============================================================================

class AnimatedBackgroundItem
{
public:
   AnimatedBackgroundItem();
   virtual ~AnimatedBackgroundItem() {}
   void Init(AnimatedBackground* _background);
   void Init(JSONValue* specifications, AnimatedBackground* _background);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of this item.
   const char* NameGet() { return name; }

   /// Draw the item.  Pass the current time in the AnimatedBackground in milliseconds.
   /// Certain types of 'AnimatedBackgroundItem' may not be able to show
   /// arbitrary times.
   virtual void Draw(unsigned int currentTime) { (void)currentTime; }
   
   /// Return the coefficient to be multipled by the main parallax offset to
   /// determine how much to offset the position of this item.
   virtual float ParallaxCoefficientGet() { return parallaxCoefficient; }

protected:
   /// Name of this item.
   const char* name;
   /// Coefficient to be multipled by the main parallax offset to determine how
   /// much to offset the position of this item.
   float parallaxCoefficient;
   /// Pointer to the AnimatedBackground to which this item belongs.
   AnimatedBackground* background;
   /// If this item should have an independent loop duration from the
   /// background, this is the length of that animation loop in milliseconds.
   /// If this item should defer to the loop duration and time of the
   /// background, this should be 0.  This is not necessarily followed by all
   /// item types.
   unsigned int durationIndependent;
   /// If using an independent duration, this keeps track of this item's
   /// independent time.
   unsigned int timeIndependent;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__ANIMATEDBACKGROUNDITEM_H__
