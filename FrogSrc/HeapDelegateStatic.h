#ifndef __FROG__HEAPDELEGATESTATIC_H__
#define __FROG__HEAPDELEGATESTATIC_H__

#include "FrogMemory.h"
#include "HeapDelegate.h"
#include "Heap.h"
#include "HeapSmall.h"
#include "Debug.h"
#include "ThreadUtilities.h"

namespace Webfoot {

/// Default maximum size for an allocation to be placed in a small heap
/// of a HeapDelegateStatic.
#define HEAP_DELEGATE_STATIC_DEFAULT_SMALL_ALLOCATION_THRESHOLD 256
/// Default name used for debug messages from HeapDelegateStatics
#define HEAP_DELEGATE_STATIC_DEFAULT_NAME "<Unspecified>"

//==============================================================================

/// HeapDelegateStatic is a HeapDelegate that does not change size after it
/// is initialized.
/// 'smallHeapCount' is the number of 256KB small allocation heaps that will
/// be used by this delegate.
template<int smallHeapCount>
class HeapDelegateStatic : public HeapDelegate
{
public:
   virtual ~HeapDelegateStatic() {}

   /// Initialize the delegate to use the specified area for its heaps.
   /// Allocations of at most '_smallAllocationThreshold' will try to be made from
   /// the HeapSmalls.  Debug messages will use the given name in reference to this instance.
   void Init(void* heapBottom, size_t size, bool _threadSafe = true,
      const char* _debugName = HEAP_DELEGATE_STATIC_DEFAULT_NAME,
      size_t _smallAllocationThreshold = HEAP_DELEGATE_STATIC_DEFAULT_SMALL_ALLOCATION_THRESHOLD);

   /// Allocate the given amount of memory in bytes.
   /// Return NULL if no memory is allocated.
   virtual void* Allocate(size_t size)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      return AllocateTop(size);
   }

   /// Free the given allocation.  This assumes the allocation was made by
   /// this HeapDelegate.  Return true if, as far as the delegate knows,
   /// the memory has been freed successfully.  Do not call this on a pointer
   /// unless you're sure it came from this delegate.
   virtual bool Deallocate(void* ptr);

   /// Allocate from the top of the heaps.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateTop(size_t size);

   /// Allocate from the bottom of the heaps.
   /// Return NULL if no memory is allocated.
   virtual void* AllocateBottom(size_t size);

   /// Return the total amount of free memory the delegate in bytes.
   virtual size_t MemoryFreeGet()
   {
      Guard guard(&(this->mutex), this->threadSafe);
      size_t freeMemory = 0;
      for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
         freeMemory += smallHeaps[smallHeapIndex].MemoryFreeGet();
      freeMemory += regularHeap.MemoryFreeGet();
      return freeMemory;
   }

   /// Return the total number of bytes used, including overhead.
   virtual size_t MemoryUsedGet()
   {
      Guard guard(&(this->mutex), this->threadSafe);
      return MemoryTotalGet() - MemoryFreeGet();
   }

   /// Return the total number of bytes in the delegate including used and free memory.
   virtual size_t MemoryTotalGet()
   {
      Guard guard(&(this->mutex), this->threadSafe);
      size_t totalMemory = 0;
      for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
         totalMemory += smallHeaps[smallHeapIndex].MemoryTotalGet();
      totalMemory += regularHeap.MemoryTotalGet();
      return totalMemory;
   }

   /// Return the total number of free blocks in the delegate.
   virtual size_t FreeBlockCountGet()
   {
      Guard guard(&(this->mutex), this->threadSafe);
      size_t freeBlockCount = 0;
      for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
         freeBlockCount += smallHeaps[smallHeapIndex].FreeBlockCountGet();
      freeBlockCount += regularHeap.FreeBlockCountGet();
      return freeBlockCount;
   }

   /// Return the size of the largest contiguous free block in the delegate in bytes.
   virtual size_t MaxFreeContiguousSizeGet()
   {
      Guard guard(&(this->mutex), this->threadSafe);
      size_t maxFreeContiguousSize = 0;
      for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
         maxFreeContiguousSize = std::max(smallHeaps[smallHeapIndex].MaxFreeContiguousSizeGet(), maxFreeContiguousSize);
      maxFreeContiguousSize = std::max(regularHeap.MaxFreeContiguousSizeGet(), maxFreeContiguousSize);
      return maxFreeContiguousSize;
   }

   typedef HeapDelegate Inherited;

protected:

   /// Use the given name for debug messages from this heap.
   /// The string will not be copied, so pass a persistent string.
#ifdef _DEBUG
   void DebugNameSet(const char* _debugName)
   {
      Guard guard(&(this->mutex), this->threadSafe);
      debugName = _debugName;
   }
#else
   void DebugNameSet(const char*) { }
#endif

   /// If it hasn't already been shown, give a warning about the regular heap being full.
   void HeapFullWarning()
   {
   #ifdef _DEBUG
      Guard guard(&(this->mutex), this->threadSafe);
      if(!gaveHeapFullWarning)
      {
         gaveHeapFullWarning = true;
         WarningPrintf("HeapDelegateStatic -- The normal heap is full in HeapDelegate: %s.\n", debugName);
      }
   #endif //#ifdef _DEBUG
   }

   /// If it hasn't already been shown, give a warning about the small heaps being full.
   void HeapSmallFullWarning()
   {
   #ifdef _DEBUG
      Guard guard(&(this->mutex), this->threadSafe);
      if(!gaveHeapSmallFullWarning)
      {
         gaveHeapSmallFullWarning = true;
         DebugPrintf("HeapDelegateStatic -- The small heaps are full in HeapDelegate: %s.\n", debugName);
      }
   #endif //#ifdef _DEBUG
   }

   /// Allocations bigger than this amount will go in the regular heap.
   /// The others will go in a small heap.
   size_t smallAllocationThreshold;

#ifdef _DEBUG
   /// True if a warning has already been issued about the small heaps being full.
   bool gaveHeapSmallFullWarning;
   /// True if a warning has already been issued about the normal heap being full.
   bool gaveHeapFullWarning;
   /// Name used for debug messages about this heap.  It will not be allocated
   /// by this class, so it will not be freed.
   const char* debugName;
#endif //#ifdef _DEBUG

   /// Heap used for allocations not appropriate for the small heaps.
   Heap regularHeap;
   /// Heaps reserved for particularly small allocations.
   HeapSmall smallHeaps[smallHeapCount];
};

//------------------------------------------------------------------------------

template<int smallHeapCount>
void HeapDelegateStatic<smallHeapCount>::Init(void* heapBottom, size_t size,
   bool _threadSafe, const char* _debugName, size_t _smallAllocationThreshold)
{
   Inherited::Init(_threadSafe);

#ifdef _DEBUG
   gaveHeapFullWarning = false;
   gaveHeapSmallFullWarning = false;
#endif

   DebugNameSet(_debugName);

   smallAllocationThreshold = _smallAllocationThreshold;
   unsigned char* currentPoint = (unsigned char*)heapBottom;
   size_t remainingSize = size;

   // Distribute memory to the small heaps.  Each one gets the maximum amount.
   for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
   {
      // Make sure we still have room.
      if(remainingSize < HEAP_SMALL_SIZE_MAX)
      {
      #ifdef _DEBUG
         ErrorPrintf("HeapDelegateStatic::Init -- '%d' is not enough memory for '%d' %d-byte small heaps. (delegate: %s)\n",
            size, smallHeapCount, HEAP_SMALL_SIZE_MAX, debugName);
      #endif //#ifdef _DEBUG
         assert(remainingSize >= HEAP_SMALL_SIZE_MAX);
         return;
      }

      smallHeaps[smallHeapIndex].Init(currentPoint, HEAP_SMALL_SIZE_MAX);
      
      remainingSize -= HEAP_SMALL_SIZE_MAX;
      currentPoint += HEAP_SMALL_SIZE_MAX;
   }

   // Give the rest of the memory to the regular heap.
   if(remainingSize < HEAP_MINIMUM_SIZE)
   {
   #ifdef _DEBUG
      ErrorPrintf("HeapDelegateStatic::Init -- Not enough memory remaining for the regular heap. (delegate: %s)\n", debugName);
   #endif //#ifdef _DEBUG
      assert(remainingSize >= HEAP_MINIMUM_SIZE);
      return;
   }
   regularHeap.Init(currentPoint, remainingSize);
}

//------------------------------------------------------------------------------

template<int smallHeapCount>
bool HeapDelegateStatic<smallHeapCount>::Deallocate(void* ptr)
{
   Guard guard(&(this->mutex), this->threadSafe);

   // First, see if it's in the small heaps.
   for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
   {
      HeapSmall* smallHeap = &(smallHeaps[smallHeapIndex]);
      if(smallHeap->Contains(ptr))
      {
         PreDeallocate(ptr);
         smallHeap->Deallocate(ptr);
         return true;
      }
   }

   // See if it's in the regular heaps.
   if(regularHeap.Contains(ptr))
   {
      PreDeallocate(ptr);
      regularHeap.Deallocate(ptr);
      return true;
   }

   return false;
}

//------------------------------------------------------------------------------

template<int smallHeapCount>
void* HeapDelegateStatic<smallHeapCount>::AllocateTop(size_t size)
{
   Guard guard(&(this->mutex), this->threadSafe);

   void* ptr;
   if(size < smallAllocationThreshold)
   {
      for(int smallHeapIndex = (smallHeapCount - 1); smallHeapIndex >= 0; smallHeapIndex--)
      {
         HeapSmall* smallHeap = &(smallHeaps[smallHeapIndex]);
         ptr = smallHeap->AllocateTop(size);
         if(ptr)
         {
            PostAllocate(ptr);
            return ptr;
         }
      }

      HeapSmallFullWarning();
   }

   ptr = regularHeap.AllocateTop(size);
   if(ptr)
      PostAllocate(ptr);
   else
      HeapFullWarning();
   return ptr;
}

//------------------------------------------------------------------------------

template<int smallHeapCount>
void* HeapDelegateStatic<smallHeapCount>::AllocateBottom(size_t size)
{
   Guard guard(&(this->mutex), this->threadSafe);

   void* ptr;
   if(size < smallAllocationThreshold)
   {
      for(int smallHeapIndex = 0; smallHeapIndex < smallHeapCount; smallHeapIndex++)
      {
         HeapSmall* smallHeap = &(smallHeaps[smallHeapIndex]);
         ptr = smallHeap->AllocateBottom(size);
         if(ptr)
         {
            PostAllocate(ptr);
            return ptr;
         }
      }

      HeapSmallFullWarning();
   }

   ptr = regularHeap.AllocateBottom(size);
   if(ptr)
      PostAllocate(ptr);
   else
      HeapFullWarning();
   return ptr;
}

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAPDELEGATESTATIC_H__
