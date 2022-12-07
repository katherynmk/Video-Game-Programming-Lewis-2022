#ifndef __FROG__HEAP_H__
#define __FROG__HEAP_H__

#include "FrogMemory.h"

namespace Webfoot {

/// When initializeing a Heap, you must provide at least this much memory.
#define HEAP_MINIMUM_SIZE (sizeof(HeapBlockHeader))

/// If true, Heap will determine the amount of free memory by counting the
/// free blocks rather than relying on keeping track of the running total.
#define HEAP_USE_SIMPLE_MEMORY_FREE 0

//===============================================================================

/// Each block of memory has this header when memory addresses are 32-bit:
///
/// 00        04        08        0C        10
/// +---------+---------+---------+---------+
/// | next    | length  | last    |used     |
/// +---------+---------+---------+---------+
///
/// Values are all stored as number of bytes.
/// length does NOT include the part of the header shown above.
///
/// If used == 0, then there is an additional section:
///
/// 00        04        08        0C        10        14        18
/// +---------+---------+---------+---------+---------+---------+
/// | next    | length  | last    |used     |nextFree |lastFree |
/// +---------+---------+---------+---------+---------+---------+
///
/// Note that the free list is sorted in memory.
///
struct HeapBlockHeader
{
   HeapBlockHeader* next;
   size_t length;
   HeapBlockHeader* last;
   size_t used;

   union
   {
      HeapBlockHeader* nextFree;
      unsigned char data[sizeof(HeapBlockHeader*)];
   };
   HeapBlockHeader* lastFree;
};

//===============================================================================

/// Heap is a general-purpose memory heap.  You can allocate from the front,
/// the back, or from whatever block fits best.  With 32-bit addresses,
/// the overhead for a single allocation is 16-bytes and it can manage
/// up to 4GB of memory.
class Heap
{
public:
   /// Initialize the heap to use the given block of memory.
   /// '_heapBottom' must be 4-byte aligned.
   void Init(void* _heapBottom, size_t _totalSize);

   /// Allocate from the free block in the heap that is closest to the requested size.
   void* Allocate(size_t size);
   /// Allocate from the bottom-most free block in the heap that is big enough.
   void* AllocateBottom(size_t size);
   /// Allocate from the top-most free block in heap that is big enough.
   void* AllocateTop(size_t size);
   /// Free the given memory allocated from this address.  This does not check
   /// to make sure the allocation came from this heap.
   void Deallocate(void *p);

   /// Return the address of the bottom of the heap.
   void* HeapBottomGet() { return (void*)heapBottom; }
   /// Return the address of the top of the heap.
   void* HeapTopGet() { return (void*)heapTop; }

   /// Return true if 'ptr' points to an address in the heap.
   /// This does not necessarily mean it was returned by an Allocate function.
   bool Contains(void* ptr)
   {
      return ((ptr >= heapBottom) && (ptr <= heapTop));
   }

   /// Return the total amount of free memory in bytes.
#if HEAP_USE_SIMPLE_MEMORY_FREE
   size_t MemoryFreeGet();
#else
   size_t MemoryFreeGet() { return currentAmountFree; }
#endif
   /// Return the total number of bytes used, including overhead.
   size_t MemoryUsedGet() { return MemoryTotalGet() - MemoryFreeGet(); }
   /// Return the total number of bytes including used and free memory.
   size_t MemoryTotalGet() { return sizeTotal; }
   /// Return the total number of free blocks.
   size_t FreeBlockCountGet();
   /// Return the size of the largest contiguous free block in bytes.
   size_t MaxFreeContiguousSizeGet();

protected:
   /// Bottom-most address for the heap
   unsigned char* heapBottom;
   /// Top-most address for the heap
   unsigned char* heapTop;
   /// Size of the region used by the heap excluding the class itself.
   size_t sizeTotalEffective;
   /// Size of the region allotted to the heap excluding the class itself.
   size_t sizeTotal;
   /// The current amount of free memory in the heap.
   size_t currentAmountFree;

   /// Bottom-most free header in the heap.
   HeapBlockHeader *freeListHead;
   /// Top-most free header in the heap.
   HeapBlockHeader *freeListTail;

   inline HeapBlockHeader* MemoryBlockFromOffsetGet(int offset);

   /// Allocate 'size' bytes.  If 'bestFit' is true, allocate from the heap block
   /// that is closest in size to the requested size.  Otherwise, allocate
   /// from the back-most block in the heap that is big enough.
   void* AllocateTopHelper(size_t size, bool bestFit);
   /// Remove the given block from the free list and mark it as used.
   void ConvertFreeToUsed(HeapBlockHeader* header);
   /// Assuming 'header' has no adjacent free blocks, add it to the free list
   /// in its address-sorted position.
   void FreeListAdd(HeapBlockHeader *header);
   /// Simply make 'header' include 'nextHeader' assuming that 'nextHeader' directly
   /// follows 'header'.
   void MergeForward(HeapBlockHeader *header, HeapBlockHeader *nextHeader);
   /// Merge 'header' with 'nextHeader' assuming that 'nextHeader' is already free,
   /// correctly linked, and directly follows 'header'
   void MergeNextFree(HeapBlockHeader *header, HeapBlockHeader *nextHeader);
};

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAP_H__
