#ifndef __FROG__ALLOCATOR_H__
#define __FROG__ALLOCATOR_H__

#include "FrogMemory.h"
#include <stddef.h> //size_t

namespace Webfoot {

/// This is called automatically by Memory::Init to set up the allocators as early as needed.
/// This will set theAllocatorBitmapData and theAllocatorSoundData to AllocatorDefault,
/// so be sure to override this in ProjectMemoryInit() if needed.
void AllocatorsInit();

//===============================================================================

/// Allocators are used to encapsulate a strategy for allocating and freeing
/// memory into an object with a very simple interface.
/// They can use a particular combination of HeapID and alignment.
/// They can also circumvent the logging system or bypass the rest of the Frog
/// memory system altogether.
class Allocator
{
public:
   virtual ~Allocator() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size) = 0;
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr) = 0;
};

//===============================================================================

/// The AllocatorDefault allocates from the default heap with default alignment.
class AllocatorDefault : public Allocator
{
public:
   virtual ~AllocatorDefault() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size);
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr);
};

extern AllocatorDefault* theAllocatorDefault;

//===============================================================================

/// The AllocatorTemp allocates from the default temporary heap with default alignment.
class AllocatorTemp : public Allocator
{
public:
   virtual ~AllocatorTemp() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size);
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr);
};

extern AllocatorTemp* theAllocatorTemp;

//===============================================================================

/// Allocates from the debug heap with default alignment without logging the allocation.
class AllocatorDebugUnlogged : public Allocator
{
public:
   virtual ~AllocatorDebugUnlogged() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size);
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr);
};

extern AllocatorDebugUnlogged* theAllocatorDebugUnlogged;

//===============================================================================

/// Allocates from the associated HeapDelegate without adjusting the alignment and
/// without logging the allocation.
class AllocatorHeapDelegateUnlogged : public Allocator
{
public:
   AllocatorHeapDelegateUnlogged() { heapDelegate = NULL; }
   virtual ~AllocatorHeapDelegateUnlogged() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size);
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr);

   /// Set the associated heap delegate from which to draw memory.
   void HeapDelegateSet(HeapDelegate* _heapDelegate) { heapDelegate = _heapDelegate; }

protected:
   /// HeapDelegate from which to draw memory.
   HeapDelegate* heapDelegate;
};

//===============================================================================

/// The AllocatorMallocFree simply uses the actual malloc and free commands.
/// Its allocations are not logged.
class AllocatorMallocFree : public Allocator
{
public:
   virtual ~AllocatorMallocFree() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size);
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr);
};

extern AllocatorMallocFree* theAllocatorMallocFree;

//===============================================================================

/// Allocator that draws memory from a specified heap.
class AllocatorHeap : public Allocator
{
public:
   AllocatorHeap();
   virtual ~AllocatorHeap() {}

   /// Return a pointer to a block of memory 'size' bytes in length.
   virtual void* Allocate(size_t size);
   /// Free a block of memory allocated with this allocator's 'Allocate' function.
   virtual void Deallocate(void* ptr);

   /// Set the allocator to draw from the given heap.
   void HeapSet(int _heapID) { heapID = _heapID; }
   /// Return the HeapID to which this allocator is currently set.
   int HeapGet() { return heapID; }

protected:
   /// Heap to which this allocator is currently set.
   int heapID;
};

/// Collection of singleton allocators that each point to a heap.  For example,
/// theAllocatorHeaps[HEAP_DEFAULT] would be a pointer to an allocator that draws
/// from HEAP_DEFAULT.
extern AllocatorHeap* theAllocatorHeaps[HEAP_COUNT];

//===============================================================================

/// Default allocator to use for Bitmap data.  Certain systems, like the
/// ImageManager, may or may not use it by default.
extern Allocator* theAllocatorBitmapData;

//===============================================================================

/// Default allocator to use for native vertex and index buffers.  Certain
/// platforms will not actually use these buffers directly as native buffers.
extern Allocator* theAllocatorGeometryData;

//===============================================================================

/// Default allocator to use for Sound data.
extern Allocator* theAllocatorSoundData;

//===============================================================================

} // namespace Webfoot {

#endif //#ifndef __FROG__ALLOCATOR_H__
