#ifndef __FROG__ACCELEROMETER_H__
#define __FROG__ACCELEROMETER_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Utility.h"

namespace Webfoot {

//==============================================================================

/// A simple interface for reacting to input from an accelerometer.
/// Positive x is right, positive y is forward, and positive z is up.
/// Acceleration magnitudes are in g's.  For example, a phone sitting face-down
/// on a table should have an acceleration of roughly (0.0f, 1.0f, 0.0f).
class Accelerometer
{
public:
   /// Initialize the cursor to be at the given position.  Keep the cursor
   /// within the given region.
 	void Init(const Point3F& _acceleration = Point3F::Create(0.0f, 0.0f, 1.0f));
   
   /// Called once per frame.
   void Update();
   
   /// Return the current vector for acceleration
   Point3F AccelerationGet() { return acceleration; }
   /// Set the current vector for acceleration
   void AccelerationSet(const Point3F& _acceleration) { acceleration = _acceleration; }
   
   /// Called by the manager to set the acceleration to use on the next update.
   void AccelerationNextSet(const Point3F& _accelerationNext) { accelerationNext = _accelerationNext; }

protected:   
   /// Acceleration for the current frame.
   Point3F acceleration;
   /// Acceleration for the next update.
   Point3F accelerationNext;
};

extern Accelerometer* theAccelerometer;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__ACCELEROMETER_H__
