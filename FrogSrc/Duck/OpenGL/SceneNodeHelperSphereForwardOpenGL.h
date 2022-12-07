#ifndef __FROG__DUCK__OPENGL__SCENENODEHELPERSPHEREFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODEHELPERSPHEREFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "Duck/SceneNodeHelperSphere.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Helper class for drawing helper spheres in Duck.
class SceneNodeHelperSphereDrawableForwardOpenGL : public SceneNodeHelperSphereDrawable
{
public:
   virtual void Draw();

   typedef SceneNodeHelperSphereDrawable Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODEHELPERSPHEREFORWARDOPENGL_H__
