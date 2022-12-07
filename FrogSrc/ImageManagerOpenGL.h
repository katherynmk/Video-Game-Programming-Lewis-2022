#ifndef __FROG__IMAGEMANAGEROPENGL_H__
#define __FROG__IMAGEMANAGEROPENGL_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Allocator.h"
#include "Bitmap.h"
#include "BitmapManager.h"
#include "FileManager.h"
#include "Image.h"
#include "ImageManagerCommon.h"

namespace Webfoot {

//==============================================================================

/// ImageManagerOpenGL is a simple interface for loading bitmap data from files
/// such that it can easily be used for drawing.
class ImageManagerOpenGL : public ImageManagerCommon
{
public:
   virtual ~ImageManagerOpenGL() {}

   /// Load the given Image from the given file.  Do not provide an extension
   /// on the filename.  Objects will be allocated from 'objectHeap'
   /// while the bitmap data will come from 'dataAllocator'.  Temporary
   /// allocations will be made from 'tempHeap'.  'options' flags come from the
   /// ImageManagerCommon::Options enum.  In this implementation, if
   /// 'dataAllocator' is NULL when loading an image where the bitmap data is
   /// known to be temporary before loading, it will default to 'tempHeap'.
   /// Otherwise, it will default to theAllocatorBitmapData.
   Image* Load(const char* filename, int options = 0, Allocator* dataAllocator = NULL,
      HeapID objectHeap = HEAP_DEFAULT, FileManager* fileManager = theFiles,
      HeapID tempHeap = HEAP_TEMP);
   
   /// Singleton instance
   static ImageManagerOpenGL instance;

protected:
   /// Helper function to Load which creates an Image from a bitmap file, like a .png.
   Image* LoadFromBitmap(const char* filename, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap,
      bool premultiplyAlpha, bool generateMipmaps, bool pointFiltering);
      
   /// Helper function to Load for loading Images that were split in advance.
   Image* LoadOptimized(const char* filename, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap,
      bool generateMipmaps, bool pointFiltering);
   /// Helper function to LoadOptimized and LoadFromBitmap for loading Images
   /// that were split in advance.
   Image* LoadOptimizedHelper(const char* filenameWithScaleAndExtension, float imageScaleAmount, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap,
      bool generateMipmaps, bool pointFiltering);

   /// Cache the given bitmap as an optimized Image.  'requestedFilename' and
   /// 'requestedFileManager' are the filename and FileManager that were used
   /// to load the bitmap.
   void CacheBitmap(Bitmap* bitmap, const char* requestedFilename, FileManager* requestedFileManager);
};

/// Singleton instance
static ImageManagerOpenGL * const theImages = &ImageManagerOpenGL::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__IMAGEMANAGEROPENGL_H__
