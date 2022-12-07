#ifndef __FROG__MEMORY_H__
#define __FROG__MEMORY_H__

#include "Platform.h"

#include <stddef.h> //size_t

#if PLATFORM_IS_MACOSX || PLATFORM_IS_IOS || PLATFORM_IS_ANDROID || PLATFORM_IS_LINUX || PLATFORM_IS_EMSCRIPTEN
   #define OVERLOAD_NEW_AND_DELETE 0
#else
   #define OVERLOAD_NEW_AND_DELETE 1
#endif

#if PLATFORM_IS_WINDOWS
   #ifdef _DEBUG
      /// If FROG_LOG_MEMORY is set, Frog will attempt to log all allocations
      /// made with frog_new, frog_new_ex, FrogMalloc, or FrogMallocEx
      /// as long as logging is enabled in theMemory.
      #define FROG_LOG_MEMORY 1
   #else
      #define FROG_LOG_MEMORY 0
   #endif //#ifdef _DEBUG
#else
   #define FROG_LOG_MEMORY 0
#endif //#if PLATFORM_IS_WINDOWS

/// Tracking the maximum amount of memory used slows the system down
/// noticeably, so this is disabled by default.
#define FROG_TRACK_MEMORY_USED_MAX 0

//#if PLATFORM_IS_WINDOWS && FROG_LOG_MEMORY
//   // When overloading 'new' in a Windows debug build, make sure stdlib.h is 
//   // included before crtdbg.h.
//   // http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=99187
//   #include <stdlib.h>
//   #include <crtdbg.h>
//
//   // When setting a macro for 'new' in Windows, you need to include xiosbase
//   // before setting the macro for new or you'll get errors.
//   //#include <xiosbase>
//#endif // #if PLATFORM_IS_WINDOWS && FROG_LOG_MEMORY

namespace Webfoot {

class HeapDelegate;
class AllocatorHeap;

/// By default, all allocations are aligned to the number of bytes in a pointer.
#define FROG_MEM_ALIGN (sizeof(void*))

//===============================================================================

/// List of heaps from which memory can be allocated.
/// If this is changed, be sure to update DEBUG_HEAP_NAMES in Memory.cpp
enum HeapID
{
   /// Heap from which allocations are made when one isn't specified
   HEAP_DEFAULT,
   /// Other end of the default heap
   HEAP_TEMP,
   /// Memory reserved for debugging.  Some dev kits have extra memory for this purpose.
   HEAP_DEBUG,
   /// MEM2 memory on the Wii
   HEAP_WII_MEM2,
   /// Other end of the MEM2 heap
   HEAP_WII_MEM2_TEMP,
   /// "Device memory" region of the 3DS
   HEAP_3DS_DEVICE_MEMORY,
   /// Other end of the "Device memory" region of the 3DS
   HEAP_3DS_DEVICE_MEMORY_TEMP,
   /// MEM1 memory of the Wii U (fast graphics memory)
   HEAP_WIIU_MEM1,
   /// Foreground heap of the Wii U
   HEAP_WIIU_FG,
   /// Separate heap for the sound thread.
   HEAP_SOUND_THREAD,
   /// Number of nominal heaps
   HEAP_COUNT,
   /// MEM1 memory on the Wii
   HEAP_WII_MEM1 = HEAP_DEFAULT,
   /// Other end of the Wii's MEM1 heap
   HEAP_WII_MEM1_TEMP = HEAP_TEMP,
   /// MEM2 memory on the Wii U
   HEAP_WIIU_MEM2 = HEAP_DEFAULT,
   /// Other end of the Wii U's MEM2 heap
   HEAP_WIIU_MEM2_TEMP = HEAP_TEMP
};

//===============================================================================

/// This must be defined by each Frog project.
/// It is called by Memory::Init() to prepare the memory.
/// Be sure to set theAllocatorBitmapData and theAllocatorSoundData here if
/// theAllocatorDefault wouldn't be appropriate for that.
void ProjectMemoryInit();
/// This must be defined by each Frog project.
/// It is called by Memory::Deinit() to clean up the memory.
void ProjectMemoryDeinit();

/// Initialize the memory system.  This can be called explicitly by the application
/// or it can be called implicitly by something like frog_new or FrogMalloc.
void FrogMemoryInit();
/// Called by the application to clean up the memory system.  Nothing can be
/// deallocated after this is called without calling FrogMemoryInit again, so
/// be sure to call this as late as possible.
void FrogMemoryDeinit();

/// Allocate the given amount of space from the specified heap with the given alignment.
/// This is only intended to be called through the FrogMalloc and FrogMallocEx macros.
/// 'alignment' must be a power of 2.
void* _FrogMallocEx(size_t size, HeapID heapId, int alignment);
/// Logged version of _FrogMallocEx that additionally records the filename and line number.
void* _FrogMallocExLogged(size_t size, HeapID heapId, int alignment, const char* file, int line);
/// Free an allocation made with FrogMalloc or FrogMallocEx.
void FrogFree(void* ptr);

/// Reallocate the given memory to the new size, alignment, and heap.
/// For now, this always reallocates and copies.  'alignment' must be a power of 2.
void* _FrogReallocEx(void* ptr, size_t size, HeapID heapId, int alignment);
/// Logged version of _FrogMallocEx that additionally records the filename and line number.
void* _FrogReallocExLogged(void* ptr, size_t size, HeapID heapId, int alignment, const char* file, int line);

/// Allocate the given number of items of the given size at the specified alignment and heap.
/// The memory will be initialized to all zeros.  'alignment' must be a power of 2.
void* _FrogCallocEx(size_t count, size_t size, HeapID heapId, int alignment);
/// Logged version of _FrogCallocEx that additionally records the filename and line number.
void* _FrogCallocExLogged(size_t count, size_t size, HeapID heapId, int alignment, const char* file, int line);

#if FROG_LOG_MEMORY
   /// Allocate the given amount from the default heap with the default alignment.
   #define FrogMalloc(size) _FrogMallocExLogged(size, Webfoot::HEAP_DEFAULT, FROG_MEM_ALIGN, __FILE__, __LINE__)
   /// Allocate the given amount from the given heap with the specified alignment.
   /// 'alignment' must be a power of 2.
   #define FrogMallocEx(size, heapId, alignment) _FrogMallocExLogged(size, heapId, alignment, __FILE__, __LINE__)

   /// Reallocate the given memory to the new size with the default alignment and heap.
   #define FrogRealloc(ptr, size) _FrogReallocExLogged(ptr, size, Webfoot::HEAP_DEFAULT, FROG_MEM_ALIGN, __FILE__, __LINE__)
   /// Reallocate the given memory to the new size, alignment, and heap.
   /// 'alignment' must be a power of 2.
   #define FrogReallocEx(ptr, size, heapID, alignment) _FrogReallocExLogged(ptr, size, heapID, alignment, __FILE__, __LINE__)

   /// Allocate the given number of items of the given size at the default alignment and heap.
   /// The memory will be initialized to all zeros.
   #define FrogCalloc(count, size) _FrogCallocExLogged(count, size, Webfoot::HEAP_DEFAULT, FROG_MEM_ALIGN, __FILE__, __LINE__)
   /// Allocate the given number of items of the given size at the specified alignment and heap.
   /// The memory will be initialized to all zeros.  'alignment' must be a power of 2.
   #define FrogCallocEx(count, size, heapId, alignment) _FrogCallocExLogged(count, size, heapId, alignment, __FILE__, __LINE__)
#else //#if FROG_LOG_MEMORY
   /// Allocate the given amount from the default heap with the default alignment.   
   #define FrogMalloc(size) _FrogMallocEx(size, Webfoot::HEAP_DEFAULT, FROG_MEM_ALIGN)
   /// Allocate the given amount from the given heap with the specified alignment.
   /// 'alignment' must be a power of 2.
   #define FrogMallocEx _FrogMallocEx

   /// Reallocate the given memory to the new size with the default alignment and heap.
   #define FrogRealloc(ptr, size) _FrogReallocEx(ptr, size, Webfoot::HEAP_DEFAULT, FROG_MEM_ALIGN)
   /// Reallocate the given memory to the new size, alignment, and heap.
   /// 'alignment' must be a power of 2.
   #define FrogReallocEx _FrogReallocEx

   /// Allocate the given number of items of the given size at the default alignment and heap.
   /// The memory will be initialized to all zeros.
   #define FrogCalloc(count, size) _FrogCallocEx(count, size, Webfoot::HEAP_DEFAULT, FROG_MEM_ALIGN)
   /// Allocate the given number of items of the given size at the specified alignment and heap.
   /// The memory will be initialized to all zeros.  'alignment' must be a power of 2.
   #define FrogCallocEx(count, size, heapId, alignment) _FrogCallocEx(count, size, heapId, alignment)
#endif //#if FROG_LOG_MEMORY

//=============================================================================

// Some platforms don't overload new and delete
#if OVERLOAD_NEW_AND_DELETE
   #if FROG_LOG_MEMORY
      /// Use frog_new just like the regular new operator when you want to use
      /// the default heap.  For example...
      ///  ExampleClass* example = frog_new ExampleClass();
      #define frog_new new(HEAP_DEFAULT, __FILE__, __LINE__)
      /// Use frog_new_ex when you want to create an object using memory from a
      /// specific heap.  For example, if you wanted to create an instance of
      /// ExampleClass in HEAP_WII_MEM2, you would call...
      ///  ExampleClass* example = frog_new_ex(HEAP_WII_MEM2) ExampleClass();
      #define frog_new_ex(_heap) new(_heap, __FILE__, __LINE__)
   #else // #if FROG_LOG_MEMORY
      #define frog_new new
      #define frog_new_ex new
   #endif // #if FROG_LOG_MEMORY
#else // #if OVERLOAD_NEW_AND_DELETE
   #define frog_new new
   #define frog_new_ex(_heap) new
#endif // #if OVERLOAD_NEW_AND_DELETE

/// Destroy and free an allocation made with frog_new or frog_new_ex.
#define frog_delete delete

} //namespace Webfoot {

// Overloading new and delete must be either global or at the class level.
// It can't simply be part of a namespace.
#if OVERLOAD_NEW_AND_DELETE
   #include "MemoryOverload.h"
#endif // #if OVERLOAD_NEW_AND_DELETE
#include "Port.h"

namespace Webfoot {

//===============================================================================

/// Keeps track of a single allocation
struct AllocationEntry
{
   /// Address of the allocation seen by the application.
   void* address;
   /// Size the allocation in bytes
   size_t size;
   /// Source file from which the allocation was made
   const char* file;
   /// Line number in the source file from which the allocation was made
   int line;
   /// Byte alignment of the allocation
   int alignment;
   /// ID of the heap that was requested.  This is not necessarily the heap
   /// where it was actually allocated.
   HeapID heapId;
};

//===============================================================================

/// Memory is the interface for allocation and freeing of memory from the main heaps.
/// It provides guard bytes, allocation allignment, and allocation logging.
/// The frog_new, frog_delete, and frog_new_ex macros use this class's singleton instance.
/// The class itself should work as long as pointers are 32-bit or bigger assuming ints
/// a still 32-bit.  All HeapDelegates should be prepared as early as possible and
/// before any allocations are made through the Memory system.
class Memory
{
public:
   Memory();

   /// Initialize the memory management system.  If the memory system is not
   /// initialized when an allocation is attempted, this will be called automatically.
   /// The memory system should only be considered thread-safe after a call to this has returned.
   void Init();
   /// Clean up the memory management system.  Only call this after all additional threads have stopped
   /// and all allocations have, in theory, been freed.
   void Deinit();

   /// Return true if the class is between Init and Deinit calls.
   bool InitializedCheck();

   /// Set the delegate for the given heap id.
   void HeapDelegateSet(HeapID heapId, HeapDelegate* heapDelegate);
   /// Return the heap delegate at the given heap id.
   /// Warning: Only use this if you know what you're doing.
   HeapDelegate* HeapDelegateGet(HeapID heapId) { return heapDelegates[heapId]; }

   /// Allocate a block of the given number of bytes from the specified heap.  Use the specified
   /// byte alignment, which must be a power of 2.
   /// This should only be called by other parts of the memory system.
   /// For general purposes, use frog_new, frog_new_ex, FrogMalloc, or FrogMallocEx instead.
   void* Allocate(size_t size, HeapID heapId = HEAP_DEFAULT, int alignment = FROG_MEM_ALIGN);
   /// Free the given allocation.
   /// This should only be called by other parts of the memory system.
   /// For general purposes, use frog_delete or FrogFree instead.
   void Deallocate(void* ptr);

   /// Return the size of the given allocation in bytes from the perspective of the application programmer.
   /// This does not include overhead.
   size_t AllocationSizeGet(void* ptr);

   /// Start tracking allocations on all the current HeapDelegates.
   /// You typically shouldn't call this until all the HeapDelegates are set.
   void LogEnable();
   /// Stop tracking allocations on all current HeapDelegates and check for leaks since tracking started.
   void LogDisable();
   /// Track the given allocation.  Only call this if the allocation was successful.
   void LogAllocation(void* ptr, size_t size, HeapID heapId, int alignment, const char* file, int line);
   /// Remove the given allocation from the log.
   void LogDeallocation(void* ptr);
   /// Print a list of all the currently logged allocations.
   void LogPrint();

   /// Check the guard bytes for the given allocation.  Return true if they are intact.
   static bool Check(void* ptr);
   /// Check the guard bytes for all logged allocations.  Return true if they are all intact.
   bool LogCheck();

   /// Return the total amount of free memory in the given delegate in bytes.
   size_t MemoryFreeGet(HeapID heapId);
   /// Return the total number of bytes used in heaps by the delegate, including overhead.
   size_t MemoryUsedGet(HeapID heapId);
   /// Return the total number of bytes used in heaps in the delegate including used and free memory.
   size_t MemoryTotalGet(HeapID heapId);
   /// Return the total number of free blocks in the given delegate.
   size_t FreeBlockCountGet(HeapID heapId);
   /// Return the size of the largest contiguous free block in the given delegate in bytes.
   size_t MaxFreeContiguousSizeGet(HeapID heapId);
   /// Return the current number of outstanding allocations from the delegate.
   size_t AllocationCountGet(HeapID heapId);
   /// Return the maximum number of outstanding allocations from the delegate so far.
   size_t AllocationCountMaxGet(HeapID heapId);
   #if FROG_TRACK_MEMORY_USED_MAX
      /// Return the maximum number of bytes used at any one time by the delegate so far.
      size_t MemoryUsedMaxGet(HeapID heapId);
   #endif //#if FROG_TRACK_MEMORY_USED_MAX

   #ifdef _DEBUG
      /// Prints stats for all heaps to the debug output.
      void StatsPrint();
   #endif //#ifdef _DEBUG

   /// Return the name of the given heap for debugging purposes.
   static const char* HeapDebugNameGet(HeapID heapId);

protected:
   /// Flag for whether the memory system is initialized.
   bool initialized;

   /// Collection of classes to manage the different nominal heaps.
   HeapDelegate* heapDelegates[HEAP_COUNT];
};

/// Singleton instance
extern Memory* theMemory;

//=============================================================================

} // namespace Webfoot

#endif //#ifndef __FROG__MEMORY_H__
