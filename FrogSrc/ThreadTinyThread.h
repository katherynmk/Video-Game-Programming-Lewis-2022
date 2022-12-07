#ifndef __FROG__THREADTINYTHREAD_H__
#define __FROG__THREADTINYTHREAD_H__

// This header should only be included from a platform's Thread.h.

#include "FrogMemory.h"
#include "tinythread.h"

#ifdef Yield
   #undef Yield
#endif

namespace Webfoot {

class ConditionVariableTinyThread;

//==============================================================================

/// Do not copy Thread objects by value.  Be aware of the scheduling policy and
/// implications of specific priority levels on your target platform.
class ThreadTinyThread
{
public:

   /// Numerically higher priority means the thread has higher priority.
   /// Be aware of the implications of a given priority level on your target
   /// platform.  Stay closer to PRIORITY_DEFAULT when practical.
   enum Priority
   {
      PRIORITY_MINUS_1,
      PRIORITY_DEFAULT,
      PRIORITY_PLUS_1,
      PRIORITY_PLUS_2,
      PRIORITY_PLUS_3
   };

   enum
   {
      STACK_SIZE_DEFAULT = 4096      
   };

   ThreadTinyThread() { internalThread = NULL; }

   typedef void (*ThreadFunction)(void*);

#if !PLATFORM_IS_WINDOWS
   /// Call this during initialization on Posix platforms to set the running thread
   /// to PRIORITY_DEFAULT.
   static void MainThreadInit();
#endif
   
   /// Prepare and start the thread.  Only use a 'priority' from the enumeration.
   /// If the system requires it, explicitly allocate a stack of the given size.
   void Init(ThreadFunction threadFunction, void* userData,
      Priority priority = PRIORITY_DEFAULT, size_t stackSize = STACK_SIZE_DEFAULT,
      HeapID heapID = HEAP_DEFAULT, int stackAlignment = FROG_MEM_ALIGN);

   /// Clean up any allocations made by this class.  Be careful not to call this
   /// until the thread is not running.
   void Deinit();
      
   /// Wait for the thread to complete and return true if successful.  This won't
   /// work on some platforms (Wii) if more than one thread attempts to join a single
   /// thread.
   void Join();
   
   /// Make the current thread sleep for the given number of milliseconds.
   static void Sleep(unsigned int duration);
   /// Allow other threads to run.
   static void Yield();

   /// Check the guard bytes of the stack and return true if everything appears fine.
   /// Not supported on all platforms.
   bool StackCheck() { return true; }

protected:
   /// Internal handle for the thread
   tthread::thread* internalThread;
};

//==============================================================================

/// A simple mutex class.
class MutexTinyThread
{
public:
   /// Prepare the mutex.
   void Init() {}
   /// Clean up the mutex.  Be sure it is not locked when you call this.
   void Deinit() {}
   
   /// Lock the mutex.
   void Lock();
   /// Unlock the mutex.
   void Unlock();
   /// If the mutex is not locked, lock it and return true.  If it is already
   /// locked, just return false.
   bool LockTry();

protected:
   /// Internal handle for the mutex
   tthread::mutex mutex;

   friend class ConditionVariableTinyThread;
};

//==============================================================================

/// A simple condition variable class
class ConditionVariableTinyThread
{
public:
   /// Prepare the condition variable.
   void Init() {}
   /// Clean up the condition variable.  Be sure it is not locked when you call this.
   void Deinit() {}

   /// Unlock the given mutex, block the current thread until Notify is called, and
   /// re-lock the mutex before returning.
   void Wait(MutexTinyThread* mutex);
   /// Wake any threads that are waiting for this condition.  Depending on the
   /// platform, if no threads are currently waiting, the next thread that
   /// attempts to Wait after this is called may or may not end up bocking.
   void Notify();

protected:
   tthread::condition_variable conditionVariable;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__THREADTINYTHREAD_H__
