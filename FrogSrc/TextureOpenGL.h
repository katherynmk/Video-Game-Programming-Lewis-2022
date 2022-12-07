#ifndef __FROG__TEXTUREOPENGL_H__
#define __FROG__TEXTUREOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "TextureCommon.h"
#include "Box2.h"
#include "Bitmap.h"

namespace Webfoot {

class ScreenOpenGL;

//==============================================================================

/// TextureOpenGL is the interface for getting 2D bitmap data to the classes that can
/// render it.  All Textures should ultimately be created by
/// theScreen->TextureCreate.
class TextureOpenGL : public TextureCommon
{
public:
   /// Initialize the texture.  Make any necessary allocations from the given heap.
   bool Init(HeapID heapID = HEAP_DEFAULT);
   /// Alternate Init used for optimized loading of Textures.
   void Init(void* _decompressedFileData, Allocator* _decompressedFileDataAllocator, HeapID _heapID);
   /// Clean up the texture object.  This will not free the Bitmaps themselves.
   void Deinit();
   
   /// Set the bitmap to use for the largest mipmap level of this texture.
   /// Be careful to only supply bitmaps of legal texture resolutions and formats.
   /// Return true if successful.
   bool BitmapSet(Bitmap* bitmap, int options = 0);
   
   /// If the bitmap previously set to this texture with BitmapSet has
   /// changed, call this to start using the latest bitmap data.  Depending
   /// on the implementation, it may already be using the latest data.
   /// Return true if successful.
   bool BitmapUpdate();

   /// If this is a running on a platform that does not need to keep the bitmap
   /// data in main memory for drawing (OpenGL, for example),
   /// free the bitmap data for this Texture.  This will not free
   /// the objects for the texture or bitmap objects, only the pixel buffer.
   void UnnecessaryBitmapDataDeallocate();
   
   /// Return the ID from OpenGL for this texture.
   GLuint TextureIDGet() { return textureID; }
   
   typedef TextureCommon Inherited;

protected:
   TextureOpenGL();

   /// Get the OpenGL parameters for working with the given bitmap format.
   /// Return true if successful.
   static bool ParametersGet(Bitmap::Format sourceBitmapFormat,
      GLint* internalFormat, GLenum* format, GLenum* type);

   /// OpenGL ID number for this texture.
   /// This will be zero if the texture is not currently registered with OpenGL.
   GLuint textureID;

   /// When loaded from an optimized file, this points to the decoded data
   /// from the file.
   void* decompressedFileData;
   /// Used to allocate and deallocate 'decompressedFileData'.
   Allocator* decompressedFileDataAllocator;

   friend class ScreenOpenGL;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TEXTUREOPENGL_H__
