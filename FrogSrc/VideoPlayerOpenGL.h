#ifndef __FROG__VIDEOPLAYEROPENGL_H__
#define __FROG__VIDEOPLAYEROPENGL_H__

// This header should only be included from a platform's VideoPlayer.h.

#include "FrogMemory.h"
#include "VideoPlayerDefault.h"

namespace Webfoot {

//==============================================================================

/// VideoPlayer implementation for OpenGL, although this may not actually be
/// suitable for all OpenGL platforms.
class VideoPlayerOpenGL : public VideoPlayerDefault
{
public:
   virtual ~VideoPlayerOpenGL() {}

   virtual void Draw();

   typedef VideoPlayerDefault Inherited;

protected:
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__VIDEOPLAYEROPENGL_H__
