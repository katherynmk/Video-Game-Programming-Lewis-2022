#ifndef __FROG__HEAPDELEGATEEXPANDABLE_H__
#define __FROG__HEAPDELEGATEEXPANDABLE_H__

#include "FrogMemory.h"
#include "HeapDelegate.h"
#include "HeapDelegateStatic.h"
#include "Allocator.h"

namespace Webfoot {

//=============================================================================

/// HeapDelegateExpandable works just like HeapDelegateStatic except that when
/// all its heaps are full, it will fall back on malloc and free for individual allocations.
template<int smallHeapCount>
class HeapDelegateExpandable : public HeapDelegateStatic<smallHeapCount>
{
public:
   virtual ~HeapDelegateExpandable() {}

   /// Allocate the given amount of memory in bytes.
   /// Return NULL if no memory is allocated.
   virtual void* Allocate(size_t size)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      void* ptr = Inherited::Allocate(size);
      if(!ptr)
         ptr = ExpandedAllocate(size);
      return ptr;
   }

   /// Free the given allocation.  This assumes the allocation was made by
   /// this HeapDelegate.  Return true if, as far as the delegate knows,
   /// the memory has been freed successfully.  Do not call this on a pointer
   /// unless you're sure it came from this delegate.
   virtual bool Deallocate(void* ptr)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      if(!Inherited::Deallocate(ptr))
         ExpandedDeallocate(ptr);

      return true;
   }

   /// Allocate from the top of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateTop(size_t size)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      void* ptr = Inherited::AllocateTop(size);
      if(!ptr)
         ptr = ExpandedAllocate(size);
      return ptr;
   }

   /// Allocate from the bottom of the delegate.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateBottom(size_t size)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      void* ptr = Inherited::AllocateBottom(size);
      if(!ptr)
        ptr = ExpandedAllocate(size);
      return ptr;
   }

   typedef HeapDelegateStatic<smallHeapCount> Inherited;

protected:
   /// When the inherited heaps are full, use this to allocate the memory.
   void* ExpandedAllocate(size_t size)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      void* ptr = theAllocatorMallocFree->Allocate(size);
      if(ptr)
         this->PostAllocate(ptr);
      return ptr;
   }

   /// When freeing memory that isn't part of the inherited heaps, use this to free the memory.
   void ExpandedDeallocate(void* ptr)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      this->PreDeallocate(ptr);
      theAllocatorMallocFree->Deallocate(ptr);
   }
};

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAPDELEGATEEXPANDABLE_H__