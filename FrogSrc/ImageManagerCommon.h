#ifndef __FROG__IMAGEMANAGERCOMMON_H__
#define __FROG__IMAGEMANAGERCOMMON_H__

#include "FrogMemory.h"
#include "Allocator.h"
#include "BitmapManager.h"
#include "Image.h"
#include "FileReferenceCounter.h"

namespace Webfoot {

//==============================================================================

/// ImageManagerCommon contains the functionality that will be common to all
/// ImageManagers implementations. 
class ImageManagerCommon
{
public:
   enum Options
   {
      /// Even if bitmap pixel data is not needed for drawing after the textures
      /// are prepared, do not free the bitmap pixel data.  For example, OpenGL
      /// makes its own copy of bitmap data, so it will be freed by default on
      /// those platforms.  However, you may need to keep the bitmap data for 
      /// some other purpose, like cloning the Image or getting the bounds of
      /// the opaque part.
      KEEP_BITMAP_DATA = 1,
      /// Don't premultiply the color channels with alpha.  This is not
      /// supported on all platforms or in all situations on a given platform.
      SKIP_PREMULTIPLY_ALPHA = 2,
      /// Generate mipmaps automatically.  This is not necessarily supported on
      /// all platforms, all formats, or all texture sizes.
      GENERATE_MIPMAPS = 4,
      /// Use point/nearest filtering instead of bilinear filtering.  This is
      /// not necessarily supported on all platforms.
      POINT_FILTERING = 8
   };

   virtual ~ImageManagerCommon() {}

   void Init(HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit();

   /// Free the given Image.
   void Unload(Image* image);

   /// Create and return an Image of the given 'dimensions'.  It should consist
   /// of Textures of legal size for the current platform with bitmap data in the
   /// specified 'bitmapFormat'.  Bitmap data will be allocated from 'dataAllocator'.
   /// objects will be allocated from 'objectHeap'.  Free Images returned by
   /// this function with BlankDeinit.  Return NULL if unsuccessful. 
   //Image* BlankInit(Bitmap::Format bitmapFormat, Point2I dimensions,
   //   Allocator* dataAllocator = theAllocatorBitmapData, HeapID objectHeap = HEAP_DEFAULT);
   /// Free an Image created with BlankInit.
   //void BlankDeinit(Image* image);

   /// Create a clone of the given Image.  Bitmap data will be allocated using
   /// 'dataAllocator'.  Objects will come from 'objectHeap'.  This can
   /// be useful when you want to modify an image with reference-counted data.
   /// This method will only work if the sourceImage still has its bitmap data.
   /// Free the object with theImages->Unload when you're done.
   Image* CloneCreate(Image* sourceImage, Allocator* dataAllocator = theAllocatorBitmapData,
      HeapID objectHeap = HEAP_DEFAULT);

   /// Create a new Image that represents a subset of another Image.  If there
   /// is no overlap, return NULL.  The new Image refers to the textures of the
   /// source Image, and no new textures or bitmap data are allocated.  When
   /// finished with the new Image, return it to theImages->Unload.  The segments
   /// of the new image will be offset by the given amount relative to the
   /// corresponding parts of the sourceImage.
   Image* SubImageCreate(Image* sourceImage,
      const Box2F& newImageSubset, const Point2F& offset,
      HeapID heapID = HEAP_DEFAULT);

protected:
   /// Collection of images
   FileReferenceCounter images;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__IMAGEMANAGERCOMMON_H__
