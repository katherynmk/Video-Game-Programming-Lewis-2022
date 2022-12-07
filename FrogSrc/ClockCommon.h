#ifndef __FROG__CLOCKCOMMON_H__
#define __FROG__CLOCKCOMMON_H__

#include "FrogMemory.h"

namespace Webfoot {

//==============================================================================

/// Base class for the Clock implementations.
class ClockCommon
{
public:
   virtual ~ClockCommon() {}     
   
   /// Call this as soon as any internal timer functions are ready.
   virtual void Init();
   
   /// Call this at the beginning of every main loop iteration to establish the official time.
   void Update();
   
   /// Return the number of milliseconds since this object was initialized.
   virtual uint32 TickCountGet() = 0;

   /// Return the official time for the current main loop iteration, in milliseconds.
   /// This is based on values returned from TickCountGet.
   uint32 LoopTickCountGet() { return loopTickCount; }
   /// Return the duration of the last main loop iteration in milliseconds.
   uint32 LoopDurationGet() { return lastLoopDuration; }
   /// Return the number of the current main loop iteration.
   uint32 LoopCountGet() { return loopCount; }

   /// Return the largest value LoopDurationGet is allowed to return.
   uint32 LoopDurationLimitGet() { return loopDurationLimit; }
   /// Set the largest value LoopDurationGet is allowed to return.
   void LoopDurationLimitSet(uint32 _loopDurationLimit) { loopDurationLimit = _loopDurationLimit; }

   /// On the next loop update, report the loop duration as zero.
   /// Call this when you run a long, blocking routine and you don't want
   /// it to cause a sudden move in all the animations on the next frame.
   void LongLoopNotify() { longLoop = true; }

protected:
   /// True if Update has not yet been called.
   bool firstUpdate;
   /// True if the loop duration should be reported as zero on the next update.
   bool longLoop;
   /// Official time for the current main loop iteration
   uint32 loopTickCount;
   /// Last value of frameTickCount.
   uint32 lastLoopTickCount;
   /// Length of the last main loop iteration in milliseconds.
   uint32 lastLoopDuration;
   /// Number of the current main loop iteration.
   uint32 loopCount;
   /// Largest value LoopDurationGet is allowed to return.
   uint32 loopDurationLimit;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CLOCKCOMMON_H__
