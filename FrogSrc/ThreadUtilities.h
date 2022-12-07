#ifndef __FROG__THREADUTILITIES_H__
#define __FROG__THREADUTILITIES_H__

#include "FrogMemory.h"
#include "Thread.h"

namespace Webfoot {

// This file contains small platform-independent utility classes for use with threads.

//==============================================================================

/// Guard will keep the given mutex locked as long as it exists.  When the guard
/// is destroyed, such as when it goes out of scope, it will automatically
/// unlock the mutex.
class Guard
{
public:
   /// If '_actuallyLock' is false, it won't actually lock.
   /// It's just to make it easy to have things that are optionally thread-safe.
   Guard(Mutex* _mutex, bool _actuallyLock = true)
   {
      mutex = _mutex;
      actuallyLock = _actuallyLock;
      if(mutex && actuallyLock)
         mutex->Lock();
   }
   
   ~Guard()
   {
      if(mutex && actuallyLock)
         mutex->Unlock();
   }
   
protected:
   Mutex* mutex;
   bool actuallyLock;
};

//==============================================================================

/// AtomicCount contains an integer and protects all access to it with a mutex.
class AtomicCount
{
public:   
   inline void Init()
   {
      value = 0;
      mutex.Init();
   }
   
   inline void Deinit()
   {
      mutex.Deinit();
   }
   
   /// Return the current value.
   inline int ValueGet()
   {
      Guard guard(&mutex);
      return value;
   }
   
   /// Set the value to '_value'.
   inline void ValueSet(int _value)
   {
      Guard guard(&mutex);
      value = _value;
   }
   
   /// Increment the value by the given amount.
   inline void Increment(int amount = 1)
   {
      Guard guard(&mutex);
      value += amount;
   }
   
   /// Decrement the value by the given amount.
   inline void Decrement(int amount = 1)
   {
      Guard guard(&mutex);
      value -= amount;
   }

protected:
   /// The stored value
   int value;
   /// Mutex used to protect the value.
   Mutex mutex;
};

//==============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__THREADUTILITIES_H__
