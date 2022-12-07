#ifndef __FROG__IMAGEOPENGL_H__
#define __FROG__IMAGEOPENGL_H__

#include "FrogMemory.h"
#include "ImageCommon.h"
#include "Color.h"

namespace Webfoot {

class ScreenOpenGL;

//=============================================================================

/// Image is the class actually used to display Textures.  It can be composed
/// of different subsets of multiple Textures.  This allows you to have
/// multiple Images share a given Texture or to have a single Image for a
/// Bitmap that needed to be split into several textures.
/// The destructor does not free anything, so be sure to call Deinit().
/// This particular implementation of Image is for use with OpenGL.
/// All Images should ultimately be created by theScreen->ImageCreate.
class ImageOpenGL : public ImageCommon
{
public:
   void Init(int _segmentCount = 1, HeapID _heapID = HEAP_DEFAULT) { Init(NULL, NULL, _segmentCount, _heapID); }
   void Init(void* _decompressedFileData, Allocator* _decompressedFileDataAllocator, int _segmentCount, HeapID _heapID);
   void Deinit();

   /// Draw the Image with its origin at 'position'.
   void Draw(const Point2F& position = Point2F::Create(0.0f, 0.0f),
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, float depth = 0.0f, float additiveBlending = 0.0f);
   /// Draw the given subset of the image with the top-left corner of that subset at 'position'.
   void Draw(const Box2F& imageSubset, const Point2F& position = Point2F::Create(0.0f, 0.0f),
      const ColorRGBA8& color = COLOR_RGBA8_WHITE, float depth = 0.0f, float additiveBlending = 0.0f);

   /// If this is running on a platform that does not need to keep the bitmap
   /// data in main memory for drawing (OpenGL, for example),
   /// free the bitmap data for the segments of this Image.  This will not free
   /// the objects for the texture or bitmap objects, only the pixel buffer.
   void UnnecessaryBitmapDataDeallocate();

   typedef ImageCommon Inherited;
   
protected:
   ImageOpenGL();
   /// Apply texture settings
   void TextureParametersApply();

   /// When loaded from an optimized file, this points to the decoded data
   /// from the file.
   void* decompressedFileData;
   /// Used to allocate and deallocate 'decompressedFileData'.
   Allocator* decompressedFileDataAllocator;
   
   friend class ScreenOpenGL;
   friend class ImageManagerOpenGL;
};

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__IMAGEOPENGL_H__
