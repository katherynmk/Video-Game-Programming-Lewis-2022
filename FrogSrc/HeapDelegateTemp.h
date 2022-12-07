#ifndef __FROG__HEAPDELEGATETEMP_H__
#define __FROG__HEAPDELEGATETEMP_H__

#include "FrogMemory.h"
#include "HeapDelegate.h"

namespace Webfoot {

//==============================================================================

/// HeapDelegateTemp is a HeapDelegate that only allocates from the
/// bottom of a different HeapDelegate.  The log will still track things
/// for this delegate separately, but all statistics, like MemoryFreeGet or 
/// MemoryUsedGet will simply return the value from the referenced delegate.
class HeapDelegateTemp : public HeapDelegate
{
public: 
   virtual ~HeapDelegateTemp() {} 

   /// Initialize the class to allocate from the given delegate.
   void Init(HeapDelegate* _referenceDelegate, bool threadSafe = true);
   
   /// Allocate the given amount of memory in bytes.
   /// Return NULL if no memory is allocated.
   virtual void* Allocate(size_t size);
   /// Free the given allocation.  This assumes the allocation was made by
   /// this HeapDelegate.  Return true if, as far as the delegate knows,
   /// the memory has been freed successfully.  Do not call this on a pointer
   /// unless you're sure it came from this delegate.
   virtual bool Deallocate(void* ptr);

   /// Allocate from the top of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateTop(size_t size);
   /// Allocate from the bottom of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateBottom(size_t size);

   /// Return true since this heap delegate is simply referencing another.
   virtual bool HeapDelegateReferencingCheck() { return true; }

   /// Return the total amount of free memory the delegate in bytes.
   virtual size_t MemoryFreeGet();
   /// Return the total number of bytes used, including overhead.
   virtual size_t MemoryUsedGet();
   /// Return the total number of bytes in the delegate including used and free memory.
   virtual size_t MemoryTotalGet();
   /// Return the total number of free blocks in the delegate.
   virtual size_t FreeBlockCountGet();
   /// Return the size of the largest contiguous free block in the delegate in bytes.
   virtual size_t MaxFreeContiguousSizeGet();
   /// Return the current number of outstanding allocations from the delegate.
   virtual size_t AllocationCountGet();
   /// Return the maximum number of outstanding allocations from the delegate so far.
   virtual size_t AllocationCountMaxGet();
   #if FROG_TRACK_MEMORY_USED_MAX
      /// Return the maximum number of bytes allocated at any one time so far.
      virtual size_t MemoryUsedMaxGet();
   #endif

   typedef HeapDelegate Inherited;
   
protected:
   /// The delegate from which this class will allocate.
   HeapDelegate* delegateReference;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAPDELEGATETEMP_H__

