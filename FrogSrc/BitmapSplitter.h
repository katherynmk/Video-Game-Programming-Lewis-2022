#ifndef __FROG__BITMAPSPLITTER_H__
#define __FROG__BITMAPSPLITTER_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Box2.h"
#include "Bitmap.h"
#include "Allocator.h"
#include "BitmapManager.h"
#include "Table.h"

namespace Webfoot {

//==============================================================================

/// Struct to represent a single segment of a bitmap after splitting into
/// other bitmaps of legal size.
struct BitmapSplitterSegment
{
   /// Position of the split bitmap relative to the origin of the original bitmap.
   Point2I position;
   /// Subset of the bitmap segment that should be visible relative to the
   /// segment's origin.
   Box2I visibleSubset;
   /// Bitmap for the segment.  
   Bitmap* bitmap;
};

//==============================================================================

/// If you have a bitmap that is not a legal size for a texture, BitmapSplitter
/// can make one or more bitmaps of legal size which can be used together to show
/// the same image.  This should only be used with fully supported
/// Bitmap::Formats where the size of a pixel is a clean number of bytes.
class BitmapSplitter
{
public:
   /// Split the given Bitmap into a a set of bitmaps where each of the new ones
   /// is a legal texture size.  Specifically, the edges will be powers of 2, 
   /// no shorter than edgeMin, and no longer than edgeMax.  New bitmap data
   /// will be allocated from 'dataAllocator'.  The bitmap objects will be allocated
   /// from 'objectHeap'.  The Table that is returned will use 'tempHeap'.
   /// 'tempHeap' will also be used for other temporary allocations.
   /// Deinit the Table, then free it with frog_delete when you are finished with it.
   /// The bitmaps in the segments exist separately from the Table and need to
   /// be Deinited and deleted individually.
   /// If no splitting was performed, return NULL.  This could mean that splitting
   /// wasn't neeed or that splitting failed.  If splitting failed, 'success' will
   /// be set to false.  So if the 'sourceBitmap' was already a legal size, 
   /// 'success' will be true and NULL will be returned.
   /// 'edgeMin', 'edgeMax', and 'edgeRemainderMax' must all be powers of 2.
   Table<BitmapSplitterSegment>* Split(Bitmap* sourceBitmap, bool* success, int edgeMin, int edgeMax,
      int edgeRemainderMax, Allocator* dataAllocator = theAllocatorBitmapData,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP);

   /// If a bitmap has already been split into segments, but the content of
   /// the source bitmap has changed, this can be used to update the content
   /// of the segment bitmaps.
   void SegmentBitmapsUpdate(Table<BitmapSplitterSegment>* segments, Bitmap* sourceBitmap);

   static BitmapSplitter instance;
            
protected:
   
   /// If Split decides that actual splitting is necessary, it is performed by SplitHelper. 
   Table<BitmapSplitterSegment>* SplitHelper(Bitmap* sourceBitmap, int edgeMin, int edgeMax,
      int edgeRemainderMax, Allocator* dataAllocator = theAllocatorBitmapData,
      HeapID objectHeap = HEAP_DEFAULT, HeapID tempHeap = HEAP_TEMP);

   /// Return a Table with one segment containing a new bitmap that is just a bigger
   /// copy of 'sourceBitmap' with given edge lengths.
   Table<BitmapSplitterSegment>* BitmapResize(Bitmap* sourceBitmap, const Point2I& newDimensions,
      Allocator* dataAllocator = theAllocatorBitmapData, HeapID objectHeap = HEAP_DEFAULT,
      HeapID tempHeap = HEAP_TEMP);
   
   /// Given the length of an edge, return the number of pieces into which it
   /// must be split along that axis. 
   int EdgeSplitCountGet(int edgeOriginal, int edgeMin, int edgeMax, int edgeRemainderMax); 
   /// For a particular axis, write the lengths into which an edge of length
   /// 'edgeOriginal' must be split.  'splitArray' must be large enough
   /// to accept all the segment edge lengths. 
   void EdgeSplitEnumerate(int32* splitArray, int32 edgeOriginal, int32 edgeMin, int32 edgeMax, int32 edgeRemainderMax);
};

BitmapSplitter* const theBitmapSplitter = &BitmapSplitter::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BITMAPSPLITTER_H__
