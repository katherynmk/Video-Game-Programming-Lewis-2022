#ifndef __FROG__SOUNDBUFFERLOADERWAV_H__
#define __FROG__SOUNDBUFFERLOADERWAV_H__

#include "FrogMemory.h"
#include "SoundBufferLoader.h"

namespace Webfoot {

//==============================================================================

/// SoundBufferLoaderWAV is an interface for loading and saving between
/// SoundBuffer objects and WAV files.
class SoundBufferLoaderWAV : public SoundBufferLoader
{
public:
   virtual ~SoundBufferLoaderWAV() {}
   
   virtual SoundBuffer* Load(const char* filename, FileManager* fileManager = theFiles,
      Allocator* dataAllocator = theAllocatorDefault,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP);
   virtual bool Save(SoundBuffer* soundBuffer, const char* filename,
      FileManager* fileManager, HeapID tempHeap = HEAP_TEMP);

   static SoundBufferLoaderWAV instance;
};

static SoundBufferLoaderWAV * const theSoundBufferLoaderWAV = &SoundBufferLoaderWAV::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SOUNDBUFFERLOADERWAV_H__
