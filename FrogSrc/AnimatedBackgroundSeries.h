#ifndef __FROG__ANIMATEDBACKGROUNDSERIES_H__
#define __FROG__ANIMATEDBACKGROUNDSERIES_H__

#include "FrogMemory.h"
#include "Table.h"
#include "Point2.h"
#include "AnimatedBackgroundItem.h"

namespace Webfoot {

class Sprite;
class AnimatedBackground;
class JSONValue;

//=============================================================================

class AnimatedBackgroundSeries : public AnimatedBackgroundItem
{
public:
   virtual ~AnimatedBackgroundSeries() {}

   /// Load the background item from the given JSONValue
   void Init(JSONValue* specifications, AnimatedBackground* _background);
   /// Set most members with a single constructor.
   void Init(unsigned int _opaqueDuration, unsigned int _fadeDuration, const Point2F& _position, AnimatedBackground* _background);
   /// Free the sprites.
   virtual void Deinit();
   
   /// Draw the item.  Pass the current time in the AnimatedBackground in milliseconds.
   virtual void Draw(unsigned int currentTime);

protected:
   /// Cache any useful information based on the current values of the members.
   void Precompute();

   /// The amount of time for a given sprite to be shown at full opacity before
   /// beginning the next fade in milliseconds
   unsigned int opaqueDuration;
   /// The length of a fade in milliseconds
   unsigned int fadeDuration;
   /// The screen position of the top-left corner.
   Point2F position;

   /// Cached length of a single opaque-fade cycle in milliseconds.
   unsigned int cycleLength;

   /// Ordered collection of the sprites to be displayed in series.
   Table<Sprite*> sprites;

   typedef AnimatedBackgroundItem Inherited;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__ANIMATEDBACKGROUNDSERIES_H__
