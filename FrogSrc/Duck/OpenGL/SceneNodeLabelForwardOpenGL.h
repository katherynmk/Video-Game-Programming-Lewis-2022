#ifndef __FROG__DUCK__OPENGL__SCENENODELABELFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODELABELFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "Duck/SceneNodeLabel.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Helper class for drawing text in Duck.
class SceneNodeLabelDrawableForwardOpenGL : public SceneNodeLabelDrawable
{
public:
   virtual void Draw();

   typedef SceneNodeLabelDrawable Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODELABELFORWARDOPENGL_H__
