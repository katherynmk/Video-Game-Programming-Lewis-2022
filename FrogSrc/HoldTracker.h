#ifndef __FROG__HOLDTRACKER_H__
#define __FROG__HOLDTRACKER_H__

#include "FrogMemory.h"

namespace Webfoot {

//=============================================================================

/// HoldTracker helps keep track of when an OnHold event has happened for a
/// given condition.  An OnHold event occurs when a condition has been true
/// on every Update for a certain amount of time in a row.  It also occurs 
/// repeatedly every time a certain interval passes after that.  This is
/// useful for having special looping behavior when the player holds down
/// a key or a button widget.  You can set whether to trigger an OnHold
/// event on updates where 'condition' is true, but it was false on the last update.
/// By default, it will trigger the event on such updates.
class HoldTracker
{
public:
   HoldTracker();
   virtual ~HoldTracker() {}

   /// Reset the HoldTracker back to its defaults and initial state.
   void Reset();

   /// 'dt' is the number of milliseconds since the last frame.
   /// 'condition' is the condition to be held.
   /// Return true if this frame is an OnHold event frame.
   bool Update(unsigned int dt, bool condition);

   /// Return the number of times Update has returned true since the last time it was
   /// called with 'condition' being false.
   int ConsecutiveHoldEventsGet() { return consecutiveHolds; }

   /// Set the number of milliseconds 'condition' must be true before
   /// OnHold events begin happening.
   void StartDelaySet(unsigned int _startDelay) { startDelay = _startDelay; }
   /// Return the number of milliseconds 'condition' must be true before
   /// OnHold events begin happening.
   unsigned int StartDelayGet() { return startDelay; }

   /// Set the number of milliseconds between repeated OnHold events.
   void IntervalDurationSet(unsigned int _intervalDuration) { intervalDuration = _intervalDuration; }
   /// Return the number of milliseconds between repeated OnHold events.
   unsigned int IntervalDurationGet() { return intervalDuration; }

   /// Set to true if an OnHold event should be triggered on updates 
   /// where 'condition' is true, but it was false on the last update.
   void TriggerEventAtStartSet(bool _triggerEventAtStart) { triggerEventAtStart = _triggerEventAtStart; }
   /// Return true if an OnHold event should be triggered on updates 
   /// where 'condition' is true, but it was false on the last update.
   bool TriggerEventAtStartGet() { return triggerEventAtStart; }

private:
   /// value of 'condition' on the last update.
   bool lastCondition;
   /// true if an OnHold event should be triggered on updates 
   /// where 'condition' is true, but it was false on the last update.
   bool triggerEventAtStart;
   /// Used to keep track of when it's time for an OnHold event.
   unsigned int holdTimer;
   /// the number of milliseconds 'condition' must be true before OnHold events begin happening.
   unsigned int startDelay;
   /// Number of milliseconds between calls to OnHold.
   unsigned int intervalDuration;
   /// Number of consecutive updates in which OnHold has been called on the same widget.
   int consecutiveHolds;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__HOLDTRACKER_H__
