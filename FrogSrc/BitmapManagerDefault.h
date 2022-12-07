#ifndef __FROG__BITMAPMANAGERDEFAULT_H__
#define __FROG__BITMAPMANAGERDEFAULT_H__

#include "FrogMemory.h"
#include "Allocator.h"
#include "BitmapManager.h"
#include "Bitmap.h"
#include "FileManager.h"
#include "BitmapLoaderFBM.h"
#include "BitmapLoaderPNG.h"

namespace Webfoot {

class Bitmap; 

//==============================================================================

/// BitmapManagerDefault is a simple interface for loading bitmap data from files.
/// While it will not always be the most efficient system for a given platform,
/// it should work everywhere.  Builds that do not wish to use BitmapManagerDefault
/// should probably not compile it and should definately not include this header. 
class BitmapManagerDefault
{
public:
   void Init() {}
   void Deinit() {}

   enum LoadOption
   {
      /// Do not create a speed-optimized version of the requested file in the
      /// cache.
      LOAD_OPTION_DO_NOT_CREATE_CACHE_FILE = 1
   };

   /// Return true if a file exists for the given bitmap.  Do not provide an
   /// extension in the filename.  If 'fullPath' is specified and the path
   /// does exist, the result of concatenating the relevant graphics path and
   /// file extension will be written to 'fullPath'.  If the path exists, but
   /// something goes wrong while writing to 'fullPath', return false.
   bool ExistsCheck(const char* filename, FileManager* fileManager = theFiles,
      char* fullPath = NULL, size_t fullPathSize = 0);

   /// Load the given Bitmap from the given file.  Do not provide an extension
   /// in the filename.  The object itself will be allocated from 'objectHeap'
   /// while the bitmap data will come from 'dataAllocator'.  Temporary
   /// allocations will be made from 'tempHeap'.
   Bitmap* Load(const char* filename, int options = 0, Allocator* dataAllocator = theAllocatorBitmapData,
      HeapID objectHeap = HEAP_DEFAULT, FileManager* fileManager = theFiles,
      HeapID tempHeap = HEAP_TEMP);
   /// Free the given Bitmap returned by Load.
   void Unload(Bitmap* bitmap);
   
   /// Return a new unitialized Bitmap of an implementation suitable for the
   /// given format.  Allocate the object for the given heap.  When you're
   /// done with the object, Deinit it and then delete it with frog_delete.
   Bitmap* BitmapCreate(Bitmap::Format format, HeapID heapID = HEAP_DEFAULT);
   
   /// Return a new bitmap in the given 'newFormat' based on the 'sourceBitmap'
   /// Not all conversions are supported and the new format may contain less data
   /// than the source format.  Return NULL if not successful.
   /// Allocate the object for the given heap.  When you're
   /// done with the object, Deinit it and then delete it with frog_delete.
   Bitmap* BitmapConvert(Bitmap::Format newFormat, Bitmap* sourceBitmap,
      HeapID objectHeap = HEAP_DEFAULT, Allocator* dataAllocator = theAllocatorBitmapData);
   
   /// Similar to BitmapConvert except it writes the converted data to 'newData'
   /// rather than creating a new Bitmap object.  Be sure 'newData' has enough
   /// memory allocated.  Return true if successful.
   bool BitmapDataConvert(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);

   /// Return a number for the time at which the given bitmap was last modified.
   /// This is not supported by all implementations of FileManager.
   int64 FileModificationTimeGet(const char* filename, FileManager* fileManager = theFiles);
   
   /// Singleton instance
   static BitmapManagerDefault instance;
   
protected:
   /// Type of function to help BitmapConvert with conversions between specific formats.
   typedef bool (*BitmapConvertHelper)(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);

   /// Helper function to Load for loading a Bitmap from an optimized format.
   Bitmap* LoadOptimized(const char* filename, int options, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap);
   /// Helper function to Load which creates a Bitmap from a normal file, like a .png.
   Bitmap* LoadUnoptimized(const char* filename, int options, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap);

   /// Cache the given bitmap as a speed-optimized file.  'requestedFilename'
   /// and 'requestedFileManager' are the filename and FileManager that were
   /// used to load the bitmap.
   void CacheBitmap(Bitmap* bitmap, const char* requestedFilename, FileManager* requestedFileManager);

   /// Return a BitmapConvertHelper that can help convert the 'sourceBitmap'
   /// to the 'newFormat'.  Return NULL if no appropriate converter is available.
   BitmapConvertHelper BitmapConvertHelperGet(Bitmap::Format newFormat, Bitmap* sourceBitmap);
   
   /// Converts a RGB8 bitmap to RGBA4.  Return true if successful.
   static bool BitmapConvertHelper_RGB8_TO_RGBA4(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGBA8 bitmap to RGBA4.  Return true if successful.
   static bool BitmapConvertHelper_RGBA8_TO_RGBA4(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGB8 bitmap to RGB565.  Return true if successful.
   static bool BitmapConvertHelper_RGB8_TO_RGB565(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGBA8 bitmap to RGB565.  Return true if successful.
   static bool BitmapConvertHelper_RGBA8_TO_RGB565(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGB8 bitmap to LA8.  Return true if successful.
   static bool BitmapConvertHelper_RGB8_TO_LA8(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGBA8 bitmap to LA8.  Return true if successful.
   static bool BitmapConvertHelper_RGBA8_TO_LA8(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGB8 bitmap to WII_RGBA8.  Return true if successful.
   static bool BitmapConvertHelper_RGB8_TO_WII_RGBA8(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
   /// Converts a RGBA8 bitmap to WII_RGBA8.  Return true if successful.
   static bool BitmapConvertHelper_RGBA8_TO_WII_RGBA8(void* newData, Bitmap::Format newFormat, Bitmap* sourceBitmap);
};

/// Singleton instance
static BitmapManagerDefault* const theBitmaps = &BitmapManagerDefault::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAPMANAGERDEFAULT_H__
