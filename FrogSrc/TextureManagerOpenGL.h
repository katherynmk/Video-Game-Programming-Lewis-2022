#ifndef __FROG__TEXTUREMANAGEROPENGL_H__
#define __FROG__TEXTUREMANAGEROPENGL_H__

#include "FrogMemory.h"
#include "Allocator.h"
#include "FileManager.h"
#include "Texture.h"
#include "TextureManagerCommon.h"

namespace Webfoot {

//==============================================================================

/// TextureManagerOpenGL is a simple interface for loading texture data from files.
class TextureManagerOpenGL : public TextureManagerCommon
{
public:   
   virtual ~TextureManagerOpenGL() {}

   void Init(HeapID heapID = HEAP_DEFAULT);
   virtual void Deinit();

   /// Load the given Texture from the given file.  Do not provide an extension
   /// on the filename.  Objects will be allocated from 'objectHeap'
   /// while the bitmap data will come from 'dataAllocator'.  Temporary
   /// allocations will be made from 'tempHeap'.  'options' flags come from the
   /// Options enum.
   Texture* Load(const char* filename, int options = 0, Allocator* dataAllocator = theAllocatorBitmapData,
      HeapID objectHeap = HEAP_DEFAULT, FileManager* fileManager = theFiles,
      HeapID tempHeap = HEAP_TEMP);

   /// Create a clone of the given Texture.  Bitmap data will be allocated using
   /// 'dataAllocator'.  Objects will come from 'objectHeap'.  This can
   /// be useful when you want to modify a texture with reference-counted data.
   /// Free the object with theTextures->Unload when you're done.
   Texture* CloneCreate(Texture* sourceTexture, Allocator* dataAllocator = theAllocatorBitmapData,
      HeapID objectHeap = HEAP_DEFAULT);
   
   /// Singleton instance
   static TextureManagerOpenGL instance;

   typedef TextureManagerCommon Inherited;

   /// Helper function shared by TextureManagerOpenGL and ImageManagerOpenGL for
   /// loading and decompressing the data in the given file.  Include the
   /// file extension.
   static void* LoadOptimizedDataHelper(const char* filename, Allocator* dataAllocator,
      FileManager* fileManager, HeapID tempHeap);

protected:
   /// Helper function to Load which creates a Texture from a bitmap file, like a .png.
   Texture* LoadFromBitmap(const char* filename, int options, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap);
      
   /// Helper function to Load for loading Textures that were optimized for this platform.
   Texture* LoadOptimized(const char* filename, int options, Allocator* dataAllocator,
      HeapID objectHeap, FileManager* fileManager, HeapID tempHeap);

   /// Cache the given bitmap as an optimized Texture.  'requestedFilename' and
   /// 'requestedFileManager' are the filename and FileManager that were used
   /// to load the bitmap.
   void CacheBitmap(Bitmap* bitmap, const char* requestedFilename, FileManager* requestedFileManager);

#if defined _DEBUG && PLATFORM_IS_WINDOWS
   /// Data stored alongside the textures for automatic reloading purposes.
   struct TextureMetadata
   {
      /// 'options' used when loading.
      int options;
      /// 'dataAllocator' used when loading.
      Allocator* dataAllocator;
      /// 'objectHeap' used when loading.
      HeapID objectHeap;
      /// 'tempHeap' used when loading.
      HeapID tempHeap;
      /// File modification time from when the texture was last loaded.
      int64 fileModificationTime;
   };

   /// Called when the program gains the focus of the OS.
   void OnOSFocusGain();
#endif
};

/// Singleton instance
static TextureManagerOpenGL * const theTextures = &TextureManagerOpenGL::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__TEXTUREMANAGEROPENGL_H__
