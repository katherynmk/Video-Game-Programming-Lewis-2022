#ifndef __FROG__BITMAPLOADERPNG_H__
#define __FROG__BITMAPLOADERPNG_H__

#include "FrogMemory.h"
#include "BitmapLoader.h"

namespace Webfoot {

//==============================================================================

/// BitmapLoader is an interface for loading and saving between Bitmaps objects
/// and PNG files.
class BitmapLoaderPNG : public BitmapLoader
{
public:
   virtual ~BitmapLoaderPNG() {}
   
   virtual Bitmap* Load(const char* filename, FileManager* fileManager = theFiles,
      Allocator* dataAllocator = theAllocatorDefault,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP);
   virtual bool Save(Bitmap* bitmap, const char* filename,
      FileManager* fileManager, HeapID tempHeap = HEAP_TEMP);

   static BitmapLoaderPNG instance;
};

static BitmapLoaderPNG * const theBitmapLoaderPNG = &BitmapLoaderPNG::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAPLOADERPNG_H__
