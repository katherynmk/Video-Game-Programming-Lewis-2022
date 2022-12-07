#ifndef __IMAGECONVERTEROPENGL_H__
#define __IMAGECONVERTEROPENGL_H__

#include "FrogMemory.h"
#include "Bitmap.h"

namespace Webfoot {

class File;
class FileManager;

//===============================================================================

/// Helper object for converting Bitmaps to optimized Images and Textures.  This
/// format is used for PC and other platforms.
class ImageConverterOpenGL
{
public:
   /// Save the given bitmap to the given filename.  If 'texture' is true,
   /// output a Texture.  If it's false, output an Image.  Return true if
   /// successful.  Include the extension for 'outputFilename'.
   static bool Save(const char* outputFilename, FileManager* fileManager, bool texture,
      Bitmap* sourceBitmap, Bitmap::Format outputBitmapFormat);

protected:
   /// Convert the image to an uncompressed .img.
   static bool SaveImage(Bitmap* sourceBitmap, const char* outputName,
      FileManager* fileManager, Bitmap::Format targetBitmapFormat);
   /// Convert the bitmap to an uncompressed .tex.
   static bool SaveTexture(Bitmap* sourceBitmap, const char* outputName,
      FileManager* fileManager, Bitmap::Format targetBitmapFormat);
   /// Write the segment bitmap's data to the given file.  Return true if
   /// successful.
   static bool SegmentBitmapDataSave(File* file, Bitmap* segmentBitmap);
   /// Return a copy of the given bitmap in the given format.
   static Bitmap* BitmapConvert(Bitmap* inputBitmap,
      Bitmap::Format targetBitmapFormat);
   /// Replace the given file with a compressed version.
   static bool FileCompress(const char* compressedFilename,
      const char* uncompressedFilename, FileManager* fileManager);
};

//===============================================================================

} // namespace Webfoot {

#endif //#ifndef __IMAGECONVERTEROPENGL_H__
