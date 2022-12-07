#ifndef __WAGEMOVINGCUBE_H__
#define __WAGEMOVINGCUBE_H__

#include "Frog.h"

namespace Webfoot {

//==============================================================================

class WageMovingCube : public WageCube
{
public:
   typedef WageCube Inherited;

   enum Type
   {
      /// Only the top face moves
      UP_FACE,
      /// Only the bottom face moves
      DOWN_FACE,
      /// Both faces move
      BOTH
   };

   struct Movement
   {
      /// The ending altitude of a movement
      float altitude;
      /// World units per second to get to altitude
      unsigned int speed;
      /// Delay (ms) before beginning the transition
      unsigned int delay;
   };

   WageMovingCube() : WageCube() {};
   ~WageMovingCube() {}

   virtual void Init(int _cellIndex, JSONValue* specifications);
   virtual void Deinit();

   virtual void Update(unsigned int dt);

   /// Gets the movement type
   Type TypeGet() { return type; }   
   /// Sets the movement type
   void TypeSet(Type _type) { type = _type; }

protected:
   /// Type of moving cube
   Type type;
   /// Managed array of movements
   Table<Movement> movements;
   /// Current movement
   int movementIndex;
   /// Starting altitude
   float previousAltitude;
   /// Altitude offset from start
   float currentAltitude;
   /// Transition object used to move the cube
   TimedTransitionLinear<float> transition;

private:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGEMOVINGCUBE_H__
