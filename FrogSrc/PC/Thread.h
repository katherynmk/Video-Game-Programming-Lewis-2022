#ifndef __FROG__THREAD_H__
#define __FROG__THREAD_H__

#include "FrogMemory.h"
#include "ThreadTinyThread.h"

namespace Webfoot {
   typedef ThreadTinyThread Thread;
   typedef MutexTinyThread Mutex;
   typedef ConditionVariableTinyThread ConditionVariable;
} //namespace Webfoot {

#endif //#ifndef __FROG__THREAD_H__
