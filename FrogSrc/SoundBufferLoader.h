#ifndef __FROG__SOUNDBUFFERLOADER_H__
#define __FROG__SOUNDBUFFERLOADER_H__

#include "FrogMemory.h"
#include "Allocator.h"
#include "FileManager.h"

namespace Webfoot {

class SoundBuffer;

//==============================================================================

/// SoundBufferLoader is an interface for encapsulating a way of loading and
/// saving sound data.  Each major format should have its own
/// SoundBufferLoader.  While the underlying formats will typically target
/// particular platforms, tools on the PC will need to use the
/// SoundBufferLoaders to convert to the format in question.
class SoundBufferLoader
{
public:
   virtual ~SoundBufferLoader() {}
   
   /// Load the given file and return it as a new SoundBuffer.  The SoundBuffer
   /// object is allocated from the specified 'objectHeap'.  Sound data
   /// is allocated using the 'dataAllocator' to help comply with any strict
   /// platform-specific sound memory rules.  Temporary allocations
   /// can be made from the given 'tempHeap'.  Return NULL if unsuccessful.
   /// 'filename' should include the extension.
   virtual SoundBuffer* Load(const char* filename, FileManager* fileManager = theFiles,
      Allocator* dataAllocator = theAllocatorDefault,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP) = 0;
   /// Write the SoundBuffer to the given file.  Temporary allocations
   /// can be made from the given 'tempHeap'.  Return true if successful.
   /// 'filename' should include the extension.
   virtual bool Save(SoundBuffer* soundBuffer, const char* filename,
      FileManager* fileManager, HeapID tempHeap = HEAP_TEMP) = 0;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SOUNDBUFFERLOADER_H__
