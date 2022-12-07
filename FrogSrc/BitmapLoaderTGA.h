#ifndef __FROG__BITMAPLOADERTGA_H__
#define __FROG__BITMAPLOADERTGA_H__

#include "FrogMemory.h"
#include "BitmapLoader.h"

namespace Webfoot {

//==============================================================================

/// BitmapLoader is an interface for loading and saving between Bitmaps objects
/// and TGA files.
class BitmapLoaderTGA : public BitmapLoader
{
public:
   virtual ~BitmapLoaderTGA() {}

   virtual Bitmap* Load(const char* filename, FileManager* fileManager = theFiles,
      Allocator* dataAllocator = theAllocatorDefault,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP);
   virtual bool Save(Bitmap* bitmap, const char* filename,
      FileManager* fileManager, HeapID tempHeap = HEAP_TEMP);
   
   static BitmapLoaderTGA instance;   
};

static BitmapLoaderTGA * const theBitmapLoaderTGA = &BitmapLoaderTGA::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAPLOADERTGA_H__
