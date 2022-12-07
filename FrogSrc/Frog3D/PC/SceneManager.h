#ifndef __FROG__FROG3D__SCENEMANAGER_H__
#define __FROG__FROG3D__SCENEMANAGER_H__

#include "FrogMemory.h"

namespace Webfoot {

/// Select the implementation of SceneManager to use here.
class SceneManagerOpenGL;
typedef SceneManagerOpenGL SceneManager;

} //namespace Webfoot {

#include "SceneManagerOpenGL.h" 

#endif //#ifndef __FROG__FROG3D__SCENEMANAGER_H__
