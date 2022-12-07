#ifndef __FROG__BITMAPLOADER_H__
#define __FROG__BITMAPLOADER_H__

#include "FrogMemory.h"
#include "Allocator.h"
#include "FileManager.h"

namespace Webfoot {

class Bitmap;

//==============================================================================

/// BitmapLoader is an interface for encapsulating a way of loading and saving
/// bitmap data.  Each major format should have its own BitmapLoader.  While the
/// underlying formats will typically target particular platforms, tools on the
/// PC will need to use the BitmapLoaders to convert to the format in question.
class BitmapLoader
{
public:
   virtual ~BitmapLoader() {}
   
   /// Load the given file and return it as a new Bitmap.  The Bitmap
   /// object is allocated from the specified 'objectHeap'.  Bitmap data
   /// is allocated using the 'dataAllocator' to help comply with any strict
   /// platform-specific texture memory rules.  Temporary allocations
   /// can be made from the given 'tempHeap'.  Return NULL if unsuccessful.
   /// 'filename' should include the extension.
   virtual Bitmap* Load(const char* filename, FileManager* fileManager = theFiles,
      Allocator* dataAllocator = theAllocatorDefault,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP) = 0;
   /// Write the bitmap data to the given file.  Temporary allocations
   /// can be made from the given 'tempHeap'.  Return true if successful.
   /// 'filename' should include the extension.
   virtual bool Save(Bitmap* bitmap, const char* filename,
      FileManager* fileManager, HeapID tempHeap = HEAP_TEMP) = 0;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAPLOADER_H__
