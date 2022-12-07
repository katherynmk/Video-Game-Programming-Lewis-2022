#ifndef __FROG__IMAGECOMMON_H__
#define __FROG__IMAGECOMMON_H__

#include "FrogMemory.h"
#include "Texture.h"
#include "Box2.h"
#include "Point2.h"

namespace Webfoot {

//==============================================================================

/// ImageSegment contians the details for a single piece of an Image.
/// This could be one part of a larger whole or it could be the only one in a
/// given Image.
struct ImageSegment
{
   /// The texture used for this segment
   Texture* texture;
   /// Which part of the texture is actually used for the segment
   Box2F textureSubset;
   /// Position of the texture subset relative to the Image's origin.
   Point2F position;
   /// True if the texture should be unloaded when the Image is unloaded.
   bool shouldUnloadTexture;
};

//==============================================================================

/// ImageCommon is the base class of all the platform-specific implementations
/// of Image.  This is primarily for consolidating common functionality, not
/// to provide an interface.
/// The destructor does not free anything, so be sure to call Deinit().
/// If the size or position of a segment must change, resubmit it with SegmentSet.
/// All Images should ultimately be created by theScreen->ImageCreate.
class ImageCommon
{
public:
   ImageCommon();

   /// Initialize the Image to use the given number of segments and 
   /// make necessary allocations from the given heap.
   void Init(int _segmentCount = 1, HeapID _heapID = HEAP_DEFAULT);
   /// Free any memory allocated internally.
   void Deinit();
   
   /// Return the number of segments.
   int SegmentCountGet() { return segmentCount; }
   
   /// Set the given segment to the specified index.
   /// The data in 'segment' will be copied by value.
   void SegmentSet(const ImageSegment* segment, int segmentIndex = 0);
   /// Return a reference to the segment at the given index.   
   /// In general, you should not change the segment returned
   /// or keep a pointer to the segment.
   /// Use SegmentSet if you want to make a change.
   const ImageSegment* SegmentGet(int segmentIndex = 0);

   /// Call BitmapUpdate for each Texture in the Image.
   void BitmapsUpdate();

   /// Return the bounds of the Image relative to its origin.
   /// This is determined by the positions and sizes of the segments, as well
   /// as the internal scale.  It does not pay attention to transparency.
   /// Results are cached to improve the speed of subsequent calls.
   const Box2F& BoundsGet();
   /// Return the width of the bounds of the Image.
   /// This does not pay attention to transparency.
   float WidthGet() { return BoundsGet().WidthGet(); }
   /// Return the height of the bounds of the Image.
   /// This does not pay attention to transparency.
   float HeightGet() { return BoundsGet().HeightGet(); }
   /// Return the width and height of the image.
   Point2F SizeGet() { return Point2F::Create(WidthGet(), HeightGet()); }

   /// Determine the bounds of the opaque part of the given subset of the image.
   /// Any alpha greater than zero will count.  If 'subset' is NULL, use the full image.
   /// If the bitmap data is no longer available, the whole Image will be treated
   /// as opaque.
   Box2F OpaqueBoundsGet(const Box2F* subset = NULL);
   /// Return true if the given point is at least partly opaque.  On some
   /// platforms (OpenGL, for example), this will only work if "KeepBitmapData"
   /// has been set to true for this animation.  This does not necessarily work
   /// with all underlying bitmap formats.
   bool OpaqueCheck(const Point2F& testPosition);
   
   /// If this is a running on a platform that does not need to keep the bitmap
   /// data in main memory for drawing (OpenGL, for example),
   /// free the bitmap data for the segments of this Image.  This will not free
   /// the objects for the texture or bitmap objects, only the pixel buffer.
   void UnnecessaryBitmapDataDeallocate();

   /// Set the scale amount used to help separate the logical size of an Image
   /// from the size of its internal texures in texels.  This is not
   /// necessarily supported on all platforms.
   void InternalScaleSet(const Point2F& _internalScale) { internalScale = _internalScale; boundsDirty = true; }
   /// See 'InternalScaleSet'.
   Point2F InternalScaleGet() { return internalScale; }

protected:
   /// Number of segments in the Image.
   int segmentCount;
   /// Array of image segments
   ImageSegment* segments;
   /// false if 'bounds' is up-to-date.
   bool boundsDirty;
   /// Cached boundaries of the Image without regard for transparency.
   Box2F bounds;
   /// See 'InternalScaleSet'.
   Point2F internalScale;
   /// True if point filtering should be used instead of bilinear filtering.
   bool pointFiltering;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__IMAGECOMMON_H__
 
