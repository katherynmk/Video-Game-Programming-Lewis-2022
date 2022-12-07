#ifndef __FROG__FROG3D__SCENEMANAGEROPENGL_H__
#define __FROG__FROG3D__SCENEMANAGEROPENGL_H__

#include "FrogMemory.h"
#include "SceneManagerCommon.h"

namespace Webfoot {

struct RenderData;

//=============================================================================

/// Class for OpenGL scene manager implementation.
class SceneManagerOpenGL : public SceneManagerCommon
{
public:
   void Init();
   void Deinit();
   
   static SceneManagerOpenGL instance;

   typedef SceneManagerCommon Inherited;

private:
};

SceneManagerOpenGL *const theSceneManager = &SceneManagerOpenGL::instance;

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENEMANAGEROPENGL_H__
