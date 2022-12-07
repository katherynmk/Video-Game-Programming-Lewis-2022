#ifndef __FROG__CLOCK_H__
#define __FROG__CLOCK_H__

#include "FrogMemory.h"
#include "Port.h"
#include "ClockCommon.h"

namespace Webfoot {

class DateTime;

//==============================================================================

/// 'Clock' is used to get the current time and date.
class Clock : public ClockCommon
{
public:
   virtual ~Clock() {}

   /// Call this as soon as any internal timer functions are ready.
   virtual void Init();

   /// Return the number of milliseconds since this object was initialized.
   virtual uint32 TickCountGet();

   /// Return a 32-bit value based on the current time for use as a random seed.
   uint32 RandomSeedGet();

   /// Set 'dateTime' to the current local date and time.  Return true if
   /// successful.
   bool DateTimeLocalGet(DateTime* dateTime);

   /// Return the number of seconds since the Unix epoch.
   int64 SecondsSinceEpochGet();

   /// Singleton instance
   static Clock instance;

   typedef ClockCommon Inherited;

protected:
   /// Internal time at which Init was last called.
   uint32 startTime;
};

/// Singleton instance of the Clock
static Clock * const theClock = &Clock::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CLOCK_H__
