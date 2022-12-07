#ifndef __FROG__HEAPDELEGATE_H__
#define __FROG__HEAPDELEGATE_H__

#include "FrogMemory.h"
#include "Map.h"
#include "Thread.h"

namespace Webfoot {

//===============================================================================

/// Different nominal heaps typically consist of multiple actual heaps.
/// These collections of heaps may also have different requirements on different
/// platforms for things like expansion policy, locking policy, and the
/// number and size of actual heaps of which it consists.  A HeapDelegate
/// abstracts all of this.
class HeapDelegate
{
public: 
   virtual ~HeapDelegate() {}

   void Init(bool _threadSafe = true);
   /// Called during Memory::Deinit
   virtual void Deinit();
   
   /// Allocate the given amount of memory in bytes.
   /// Return NULL if no memory is allocated.
   virtual void* Allocate(size_t size) = 0;
   /// Free the given allocation.  This assumes the allocation was made by
   /// this HeapDelegate.  Return true if, as far as the delegate knows,
   /// the memory has been freed successfully.  Do not call this on a pointer
   /// unless you're sure it came from this delegate.
   virtual bool Deallocate(void* ptr) = 0;

   /// Allocate from the top of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateTop(size_t size) = 0;
   /// Allocate from the bottom of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateBottom(size_t size) = 0;
   
   /// For statistics purposes, inheriting classes must call this function
   /// after successful allocations if they are not just reference heaps.
   virtual void PostAllocate(void* /*ptr*/)
   {
      allocationCount++;
      if(allocationCount > allocationCountMax)
         allocationCountMax = allocationCount;
      #if FROG_TRACK_MEMORY_USED_MAX
         size_t currentUsedMemory = MemoryUsedGet();
         if(currentUsedMemory > memoryUsedMax)
            memoryUsedMax = currentUsedMemory;
      #endif
   }
   /// For statistics purposes, inheriting classes must call this function
   /// before deallocations.
   virtual void PreDeallocate(void* /*ptr*/)
   {
      assert(allocationCount > 0);
      allocationCount--;
   }

   /// Reset the allocation log and begin logging.
   virtual void LogEnable();
   /// Stop logging and print all currently logged allocations.
   virtual void LogDisable();
   /// Return true if the allocation log is currently enabled.
   virtual bool LogEnabledCheck() { return logEnabled; }

   /// If logging is enabled, remember that the given allocation was made to this heap.
   virtual void LogAllocation(void* ptr, AllocationEntry* entry);
   /// If logging is enabled, remove a given allocation from the log.  Return true if the allocation was in the log.
   virtual bool LogDeallocation(void* ptr);

   /// Check the guard bytes for all logged allocations.  Return true if they are all intact.
   virtual bool LogCheck();
   /// Print a list of all the currently logged allocations.
   virtual void LogPrint();
   
   /// Return true if this heap delegate is simply referencing another.
   virtual bool HeapDelegateReferencingCheck() { return false; }

   /// Return the total amount of free memory the delegate in bytes.
   virtual size_t MemoryFreeGet() = 0;
   /// Return the total number of bytes used, including overhead.
   virtual size_t MemoryUsedGet() = 0;
   /// Return the total number of bytes in the delegate including used and free memory.
   virtual size_t MemoryTotalGet() = 0;
   /// Return the total number of free blocks in the delegate.
   virtual size_t FreeBlockCountGet() = 0;
   /// Return the size of the largest contiguous free block in the delegate in bytes.
   virtual size_t MaxFreeContiguousSizeGet() = 0;
   /// Return the current number of outstanding allocations from the delegate.
   virtual size_t AllocationCountGet();
   /// Return the maximum number of outstanding allocations from the delegate so far.
   virtual size_t AllocationCountMaxGet();
   #if FROG_TRACK_MEMORY_USED_MAX
      /// Return the maximum number of bytes allocated at any one time so far.
      virtual size_t MemoryUsedMaxGet();
   #endif //#if FROG_TRACK_MEMORY_USED_MAX
   
protected:
   bool threadSafe;
   /// True if logging should be taking place.
   bool logEnabled;
   /// Counter for the number of allocations.
   size_t allocationCount;
   /// The highest value reached by 'allocationCount'
   size_t allocationCountMax;
   #if FROG_TRACK_MEMORY_USED_MAX
      /// The maximum number of bytes allocated at any one time so far.
      size_t memoryUsedMax;
   #endif
   /// Collection of allocations made through this delegate.
   Map<void*, AllocationEntry> allocationLog;
   /// Allocator used to get memory for allocation logs.
   AllocatorHeapDelegateUnlogged logAllocator;
   /// Lock used to protect the contents of the heap.
   Mutex mutex;
};

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAPDELEGATE_H__
