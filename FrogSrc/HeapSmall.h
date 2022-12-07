#ifndef __FROG__HEAPSMALL_H__
#define __FROG__HEAPSMALL_H__

#include "FrogMemory.h"

namespace Webfoot {

/// Maximum amount of memory that can be stored in a single HeapSmall
#define HEAP_SMALL_SIZE_MAX (255*1024)
/// Minimum amount of memory that can be stored in a single HeapSmall
#define HEAP_SMALL_SIZE_MIN (sizeof(Webfoot::HeapSmallBlockHeader) + Webfoot::HeapSmall::InitialOffset)
/// Number of bytes in a HeapSmall block
#define HEAP_SMALL_BLOCK_SIZE 4

/// If true, HeapSmall will determine the amount of free memory by counting the
/// free blocks rather than relying on keeping track of the running total.
#define HEAP_SMALL_USE_SIMPLE_MEMORY_FREE_GET 0

//===============================================================================

/// Each block of memory has this header:
///
///   00   01   02   03   04   05   06   07
/// +---------+---------+---------+----+----+
/// | next    | length  | last    |used|    |
/// +---------+---------+---------+----+----+
///
/// Values are all stored as number of 4-byte blocks.
/// length does NOT include the part of the header shown above
///
/// If used == 0, then there is an additional section:
///
///   00   01   02   03   04   05   06   07   08   09   0A   0B
/// +---------+---------+---------+----+----+---------+---------+
/// | next    | length  | last    |used|    |nextFree |lastFree |
/// +---------+---------+---------+----+----+---------+---------+
///
/// Note that the free list is sorted in memory.
///
struct HeapSmallBlockHeader
{
   unsigned short next;
   unsigned short length;
   unsigned short last;
   unsigned char used;
   unsigned char _reserved;

   union
   {
      unsigned short nextFree;
      unsigned char data[2];
   };
   unsigned short lastFree;
};

//===============================================================================

/// This heap should only be used for small allocations.  The overhead per
/// allocation is 12 bytes, and the heap itself can't hold more than 256KB
/// (2^18 bytes) total.  Allocations are made in 4-byte data blocks.
/// Use multiple HeapSmalls if needed.
class HeapSmall
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
#if HEAP_SMALL_USE_SIMPLE_MEMORY_FREE_GET
   size_t MemoryFreeGet();
#else
   size_t MemoryFreeGet() { return blocksFree * HEAP_SMALL_BLOCK_SIZE; }
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
   /// Size of the heap excluding the class itself.
   size_t sizeTotal;
   /// Current number of blocks free
   size_t blocksFree;

   /// Offset for the first block
   enum { InitialOffset = 4 };

   /// Bottom-most free header in the heap.
   HeapSmallBlockHeader *freeListHead;
   /// Top-most free header in the heap.
   HeapSmallBlockHeader *freeListTail;

   //inline HeapSmallBlockHeader* MemoryBlockFromOffsetGet(int offset);
   //inline unsigned short OffsetFromMemoryBlockGet(HeapSmallBlockHeader *header);

   /// Allocate 'size' bytes.  If 'bestFit' is true, allocate from the heap block
   /// that is closest in size to the requested size.  Otherwise, allocate
   /// from the back-most block in the heap that is big enough.
   void* AllocateTopHelper(size_t size, bool bestFit);
   /// Remove the given block from the free list and mark it as used.
   void ConvertFreeToUsed(HeapSmallBlockHeader* header);
   /// Assuming 'header' has no adjacent free blocks, add it to the free list
   /// in its address-sorted position.
   void FreeListAdd(HeapSmallBlockHeader *header);
   /// Simply make 'header' include 'nextHeader' assuming that 'nextHeader' directly
   /// follows 'header'.
   void MergeForward(HeapSmallBlockHeader *header, HeapSmallBlockHeader *nextHeader);
   /// Merge 'header' with 'nextHeader' assuming that 'nextHeader' is already free,
   /// correctly linked, and directly follows 'header'
   void MergeNextFree(HeapSmallBlockHeader *header, HeapSmallBlockHeader *nextHeader);
};

//===============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__HEAPSMALL_H__
