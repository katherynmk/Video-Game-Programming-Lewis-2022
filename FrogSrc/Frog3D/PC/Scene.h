#ifndef __FROG__FROG3D__SCENE_H__
#define __FROG__FROG3D__SCENE_H__

#include "FrogMemory.h"

namespace Webfoot {

/// Select the implementation of Scene to use here.
class SceneOpenGL;
typedef SceneOpenGL Scene;

} //namespace Webfoot {

#include "SceneOpenGL.h"

#endif //#ifndef __FROG__FROG3D__SCENE_H__
