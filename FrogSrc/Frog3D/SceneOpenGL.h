#ifndef __FROG__FROG3D__SCENEOPENGL_H__
#define __FROG__FROG3D__SCENEOPENGL_H__

#include "FrogMemory.h"
#include "SceneCommon.h"

namespace Webfoot {

struct RenderData;

//=============================================================================

/// Class for OpenGL scene implementation.
class SceneOpenGL : public SceneCommon
{
public:
   void Render(RenderData* renderData);

   typedef SceneCommon Inherited;

private:
   /// Helps avoid redundant code when drawing the sky and root separately.
   void RenderHelper(RenderData* renderData, SceneNode* _rootSceneNode);
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENEOPENGL_H__
