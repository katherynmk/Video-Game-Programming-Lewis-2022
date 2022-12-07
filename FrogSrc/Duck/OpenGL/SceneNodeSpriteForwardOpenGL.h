#ifndef __FROG__DUCK__OPENGL__SCENENODESPRITEFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODESPRITEFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "Duck/SceneNodeSprite.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Helper class for drawing sprites in Duck.
class SceneNodeSpriteDrawableForwardOpenGL : public SceneNodeSpriteDrawable
{
public:
   virtual void Draw();

   typedef SceneNodeSpriteDrawable Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODESPRITEFORWARDOPENGL_H__
