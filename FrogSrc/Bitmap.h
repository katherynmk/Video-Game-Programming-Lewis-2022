#ifndef __FROG__BITMAP_H__
#define __FROG__BITMAP_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Box2.h"
#include "Port.h"
#include "Allocator.h"
#include "Utility.h"
#include "Color.h"

namespace Webfoot {

/// Widths of Bitmaps in WII_RGBA8 format must be a multiple of this number.
#define WII_RGBA8_WIDTH_MULTIPLE 4
/// Heights of Bitmaps in WII_RGBA8 format must be a multiple of this number.
#define WII_RGBA8_HEIGHT_MULTIPLE 4

//==============================================================================

/// A Bitmap stores 2D bitmap data.  The class itself does not care where the
/// data comes from.  The default implementation has no associated palette
/// and assumes that the pixel size is a clean number of bytes.
/// This means the base class is not suitable for all formats.
/// The destructor does not free anything, so be sure to call Deinit when you're done.
/// Use theBitmaps->BitmapCreate to get a new Bitmap object of the right type
/// for your format.  Beware that not all features of this class work with
/// all formats.
class Bitmap
{
public:
   Bitmap();
   virtual ~Bitmap() {}
   
   /// Enumeration of all formats used by Bitmap and classes that extend Bitmap.
   /// This includes formats intended for other platforms.
   enum Format
   {
      FORMAT_INVALID = -1,
      /// 24-bit truecolor
      FORMAT_RGB8,
      /// 32-bit truecolor including alpha
      FORMAT_RGBA8,
      /// 16-bit with 4-bits per channel.  Much of the Bitmap API does not work with this format.
      FORMAT_RGBA4,
      /// 16-bits: 5 for red, 6 for green, and 5 for blue.
      FORMAT_RGB565,
      /// 8-bits: all for luminance.   Much of the Bitmap API does not work with this format.
      FORMAT_L8,
      /// 16-bits: 8 for luminance and 8 for alpha.  Much of the Bitmap API does not work with this format.
      FORMAT_LA8,
      /// 16-bits: all for luminance.   Much of the Bitmap API does not work with this format.
      FORMAT_L16,
      /// 24-bit iPhone truecolor
      FORMAT_BGR8,
      /// 32-bit iPhone truecolor including alpha
      FORMAT_BGRA8,
      /// A form of RGBA8 for the Wii.  Much of the Bitmap API does not work with this format.
      FORMAT_WII_RGBA8,
      /// Native compressed texture for the Wii.  Only the most basic parts of the Bitmap API work with this.
      FORMAT_WII_CMPR,
      /// Native Wii format.  Only the most basic parts of the Bitmap API work with this.
      FORMAT_WII_RGB565,
      /// Native Wii format.  Only the most basic parts of the Bitmap API work with this.
      FORMAT_WII_RGB5A3,
      /// Native form of RGB8 for 3DS.  Only the most basic parts of the Bitmap API work with this.
      FORMAT_CTR_RGB8,
      /// Native form of RGBA8 for 3DS.  Only the most basic parts of the Bitmap API work with this.
      FORMAT_CTR_RGBA8,
      /// Number of formats.
      FORMAT_COUNT
   };

   /// Clean up anything that was allocated by this class.
   void Deinit();

   /// Allocate space to give the bitmap the specified dimensions and format.
   /// Return true if successful.
   bool Allocate(const Point2I& _dimensions, Format _format, Allocator* _allocator = theAllocatorDefault);
   /// Without forgetting the format or dimensions, clean up anything that was allocated by this object.
   void Deallocate();
   /// Use the given point in memory as data for a bitmap of the given
   /// dimensions and format.
   /// This would be useful for bitmaps stored in program memory or allocated
   /// externally.
   void DataSet(void* _data, const Point2I& _dimensions, Format _format);
   /// Return a duplicate of this Bitmap.  Allocate the object itself from 'objectHeap'.
   /// Allocate the bitmap data with 'dataAllocator'.  This can be useful when you need
   /// a unique instance of a Bitmap that is reference counted.  Return NULL if unsuccessful.
   virtual Bitmap* Clone(Allocator* dataAllocator = theAllocatorDefault, HeapID objectHeap = HEAP_DEFAULT);
   
   /// Return a pointer to the bitmap data.
   void* DataGet() { return data; }

   /// Return the height and width of the bitmap.
   Point2I SizeGet() { return dimensions; }
   /// Return the width of the bitmap;
   int WidthGet() { return dimensions.x; }
   /// Return the height of the bitmap;
   int HeightGet() { return dimensions.y; }
   /// Convenience function that returns a Box2I with an x,y of 0,0
   /// and the same width and height as the Bitmap.
   Box2I DimensionsBoxGet() { return Box2I::Create(0, 0, dimensions.x, dimensions.y); }

   /// Return the format of this Bitmap.
   Format FormatGet() { return format; }
   /// Return the number of bits per pixel for this Bitmap.
   int BitsPerPixelGet() { return BitsPerPixelGet(format); }
   /// Return the number of bits per pixel of the specified format.
   static int BitsPerPixelGet(Format _format);
   /// Return the number of bytes per pixel for the current Bitmap.  This is only
   /// applicable if the number of bits per pixel is a clean number of bytes.
   int BytesPerPixelGet() { return BitsToBytes(BitsPerPixelGet()); }
   /// Return the number of bytes per pixel of the specified format.  This is only
   /// applicable if the number of bits per pixel is a clean number of bytes.
   static int BytesPerPixelGet(Format _format) { return BitsToBytes(BitsPerPixelGet(_format)); }
   /// Return true if this bitmap's format includes opacity information.
   bool AlphaCheck() { return AlphaCheck(format); }
   /// Return true if the given bitmap format includes opacity information.
   static bool AlphaCheck(Format _format);

   /// Determine the bounds of the opaque part of the given subset of the image.
   /// Any alpha greater than zero will count.  If 'subset' is NULL, use the full bitmap.
   /// If the data is null, this will return the last full dimensions of the bitmap.
   virtual Box2I OpaqueBoundsGet(const Box2I* subset = NULL);

   /// Return true if the given pixel is fully black, without regard for
   /// opacity.  This may not be supported for all bitmap formats.
   virtual bool BlackCheck(const Point2I& position);

   /// Enumeration of options that can be passed to the Copy method.
   enum CopyOptions
   {
      /// Overwrite without regard for transparent pixels.
      COPY_OPAQUE = 0,
      /// Blend when pixels overlap.
      COPY_TRANSPARENT = 1
   };
   
   /// Copy the 'sourceRegion' part of 'sourceBitmap' to the 'destRegion'
   /// part of this Bitmap.  This assumes that both bitmaps are in the
   /// same format.  The base class implementation assumes a clean number
   /// of bytes per pixel.  This function will ignore transparency by default.
   /// If 'sourceRegion' is NULL, it will be attempt to copy as much of
   /// 'sourceBitmap' as possible starting from the top-left.
   /// If 'destRegion' is NULL, it will copy to the top-left corner of
   /// this bitmap.  Use 'options' to control other aspects of the copying.
   virtual void Copy(Bitmap* sourceBitmap, Box2I* destRegion = NULL, Box2I* sourceRegion = NULL, CopyOptions options = COPY_OPAQUE);
   
   /// Get the color of the given pixel.  The base class implementation assumes
   /// a clean number of bytes per pixel.
   virtual void PixelGet(void* color, const Point2I& position);
   /// Set the color of the given pixel.  The base class implementation assumes
   /// a clean number of bytes per pixel.
   virtual void PixelSet(const void* color, const Point2I& position);
   
   /// Attempt to make the whole bitmap black, and if possible, transparent too.
   /// This is not supported for all bitmap formats.
   virtual void Clear();
   /// Fill the given region with the given color.
   /// If no region is provided, fill the whole bitmap.
   /// The base implementation assumes a clean number of bytes per pixel
   virtual void Clear(const void* color, const Box2I* region = NULL);

   /// Return the address of the given point in the bitmap data.
   /// This is only applicable to formats with a clean number of bytes per pixel.
   void* PixelAddressGet(const Point2I& position);

   /// Apply the alpha channel to the color channels.
   /// This is currently only supported for the RGBA8 format.
   virtual void PremultiplyAlpha();

   /// Mirror the bitmap vertically.  This is not supported for all bitmap
   /// formats.
   void FlipVertical();

   /// Return a string representation of the given bitmap format.  Return NULL
   /// if unsuccessful.  Beware not all formats have a string representation.
   static const char* FormatCodeToString(Format _format);
   /// Return the bitmap format that correspends to the given string.  Return
   /// FORMAT_INVALID if unsuccessful.  Beware not all formats have a string
   /// representation.
   static Format FormatStringToCode(const char* formatName);

protected:

   /// Helper function to OpaqueBoundsGet that abstracts away the location
   /// of the alpha.  This assumes a clean number of bytes per pixel.
   /// An 'OpaqueCheck' function must be provided to test whether the given
   /// pixel is fully transparent.
   template<typename Pixel>
   void OpaqueBoundsGetHelper(Box2I* bounds, const Box2I* finalSubset, bool(*OpaqueCheck)(Pixel*));

   /// Helper function to BoundsGet for WII_RGBA8 bitmaps. 
   void OpaqueBoundsGetHelperWiiRGBA8(Box2I* bounds, const Box2I* finalSubset);

   /// Pointer to the actual bitmap data for this bitmap.
   void* data;
   /// Format of this bitmap's data.
   Format format;
   /// If the Allocate or Clone methods were used, this is the allocator that was used.
   /// Otherwise, it is NULL.
   Allocator* allocator;
   /// Height and width of this bitmap.
   Point2I dimensions;
};

//==============================================================================

template<typename Pixel>
void Bitmap::OpaqueBoundsGetHelper(Box2I* bounds, const Box2I* finalSubset, bool(*OpaqueCheck)(Pixel*))
{
   // Prepare to search
   bool foundPixel = false;
   int searchWidth = finalSubset->width;
   int searchHeight = finalSubset->height;
   int searchXStart = finalSubset->x;
   int searchYStart = finalSubset->y;
   int searchXLimit = searchXStart + searchWidth;
   int searchYLimit = searchYStart + searchHeight;
   int minX = searchXLimit;
   int minY = searchYLimit;
   int maxX = searchXStart - 1;
   int maxY = searchYStart - 1;
   int newLineOffset = dimensions.x - finalSubset->width;
   Point2I position = Point2I::Create(searchXStart, searchYStart);
   Pixel* searchPixel  = (Pixel*)PixelAddressGet(position);

   // Search
   for(int y = searchYStart; y < searchYLimit; y++)
   {
      for(int x = searchXStart; x < searchXLimit; x++)   
      {
         if(OpaqueCheck(searchPixel))
         {
            foundPixel = true;
            if(x < minX)
               minX = x;
            if(y < minY)
               minY = y;
            if(x > maxX)
               maxX = x;
            if(y > maxY)
               maxY = y;
         }
         searchPixel++;
      }
      searchPixel += newLineOffset;
   }

   // Report results
   if(foundPixel)
   {
      bounds->x = minX;
      bounds->y = minY;
      bounds->width = (maxX - minX) + 1;
      bounds->height = (maxY - minY) + 1;
   }
   else
   {
      bounds->EmptySet();
   }
}

//-----------------------------------------------------------------------------

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAP_H__
