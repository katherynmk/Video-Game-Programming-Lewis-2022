#ifndef __FROG__ACCELEROMETERMANAGERMOUSE_H__
#define __FROG__ACCELEROMETERMANAGERMOUSE_H__

#include "FrogMemory.h"
#include "Accelerometer.h"
#include "Point2.h"

namespace Webfoot {

//==============================================================================

/// Drag the mouse with the right mouse button held down to simulate tilting an
/// accelerometer.
class AccelerometerManagerMouse
{
public:
   void Init();
   void Deinit();
   
   void Update();

   static AccelerometerManagerMouse instance;

protected:
   Accelerometer accelerometer;
   /// Position of the cursor when the drag started.
   Point2F dragPostitionBegin;
};

AccelerometerManagerMouse* const theAccelerometerManagerMouse = &AccelerometerManagerMouse::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__ACCELEROMETERMANAGERMOUSE_H__
