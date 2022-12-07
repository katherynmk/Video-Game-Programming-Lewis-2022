#ifndef __FROG__IMAGE_H__
#define __FROG__IMAGE_H__

#include "FrogMemory.h"
#include "ImageOpenGL.h"

namespace Webfoot {
   class ImageOpenGL;
   typedef ImageOpenGL Image;
   
   /// Minimum length of the edge of an image segment.  Must be a power of 2.
   const int IMAGE_SEGMENT_EDGE_MIN = 8;
   /// Maximum length of the edge of an image segment.  Must be a power of 2.
   const int IMAGE_SEGMENT_EDGE_MAX = 2048;
   /// Maximum length of an image segment edge for the remainder of an image
   /// when some space must be wasted.  Must be a power of 2.
   const int IMAGE_SEGMENT_EDGE_REMAINDER_MAX = 256;
} //namespace Webfoot {

#endif //#ifndef __FROG__IMAGE_H__
