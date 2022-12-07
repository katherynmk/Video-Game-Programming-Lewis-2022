#ifndef __WAGEMOODSPRITE_H__
#define __WAGEMOODSPRITE_H__

#include "Frog.h"

namespace Webfoot {

//==============================================================================

class WageMoodSprite : public WageSprite
{
public:
   typedef WageSprite Inherited;

   struct Movement
   {
      /// The ending position of a movement in world units
      Point3F position;
      /// Delay (ms) before beginning the transition
      int delay;
   };

   WageMoodSprite() : WageSprite() {}
   virtual ~WageMoodSprite() {}
   
   virtual void Init(const char* resourceFilename, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);

protected:
   /// Managed array of movement structs (positions and delays)
   Table<Movement> movements;
   /// Transition object to move the sprite
   TimedTransitionLinear<Point3F> transition;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGEMOODSPRITE_H__
