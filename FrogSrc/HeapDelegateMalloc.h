#ifndef __FROG__HEAPDELEGATEMALLOC_H__
#define __FROG__HEAPDELEGATEMALLOC_H__

#include "FrogMemory.h"
#include "HeapDelegate.h"

namespace Webfoot {

//==============================================================================

/// HeapDelegateMalloc is a simple HeapDelegate for use in tools.
/// It is the default delegate for Frog, but it is not intended for use in games.
/// Most statistics members like, MemoryFreeGet and MemoryUsedGet,
/// do not return accurate values.
class HeapDelegateMalloc : public HeapDelegate
{
public:
   virtual ~HeapDelegateMalloc() {} 
   
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
   virtual void* AllocateTop(size_t size)  { return Allocate(size); }
   /// Allocate from the bottom of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateBottom(size_t size) { return Allocate(size); }

   /// Return true if this heap delegate is simply referencing another.
   virtual bool HeapDelegateReferencingCheck() { return false; }

   /// Return the total amount of free memory the delegate in bytes.
   virtual size_t MemoryFreeGet() { return 0; }
   /// Return the total number of bytes used, including overhead.
   virtual size_t MemoryUsedGet() { return 0; }
   /// Return the total number of bytes in the delegate including used and free memory.
   virtual size_t MemoryTotalGet() { return 0; }
   /// Return the total number of free blocks in the delegate.
   virtual size_t FreeBlockCountGet() { return 0; }
   /// Return the size of the largest contiguous free block in the delegate in bytes.
   virtual size_t MaxFreeContiguousSizeGet() { return 0; }

   static HeapDelegateMalloc instance;
};

//==============================================================================

HeapDelegateMalloc* const theHeapDelegateMalloc = &HeapDelegateMalloc::instance;

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAPDELEGATEMALLOC_H__
