#ifndef __FROG__BITMAPLOADERFBM_H__
#define __FROG__BITMAPLOADERFBM_H__

#include "FrogMemory.h"
#include "BitmapLoader.h"

namespace Webfoot {

//==============================================================================

/// BitmapLoaderFBM is an interface for loading and saving between Bitmaps objects
/// and Frog bitmap (FBM) files.  An FBM file consists of BSON data that was
/// saved with FileManager::CompressedFileSave.  Data in the file is generally
/// little-endian.  They don't compress as much as PNGs but they decompress faster.
class BitmapLoaderFBM : public BitmapLoader
{
public:
   virtual ~BitmapLoaderFBM() {}
   
   virtual Bitmap* Load(const char* filename, FileManager* fileManager = theFiles,
      Allocator* dataAllocator = theAllocatorDefault,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP);
   virtual bool Save(Bitmap* bitmap, const char* filename,
      FileManager* fileManager, HeapID tempHeap = HEAP_TEMP);

   static BitmapLoaderFBM instance;
};

static BitmapLoaderFBM * const theBitmapLoaderFBM = &BitmapLoaderFBM::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAPLOADERFBM_H__
