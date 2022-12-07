#ifndef __FROG__DUCK__CAMERACONTROLLERSCENENODECAMERA_H__
#define __FROG__DUCK__CAMERACONTROLLERSCENENODECAMERA_H__

#include "FrogMemory.h"
#include "Duck/CameraController.h"

namespace Webfoot {
namespace Duck {

class SceneInstance;
class SceneNodeCamera;

//==============================================================================

/// This uses a 'SceneNodeCamera', probably exported from Blender, to control
/// the camera.
class CameraControllerSceneNodeCamera : public CameraController
{
public:
   CameraControllerSceneNodeCamera();

   virtual void Deinit();

   virtual Matrix43 ViewMatrixCenterGet();
   virtual Matrix43 CameraTransformCenterGet();

   /// Set the SceneNodeCamera to use.
   void SceneNodeCameraSet(SceneNodeCamera* _sceneNodeCamera) { sceneNodeCamera = _sceneNodeCamera; }
   /// Return the SceneNodeCamera being used.
   SceneNodeCamera* SceneNodeCameraSet() { return sceneNodeCamera; }

   typedef CameraController Inherited;

private:
   /// SceneNodeCamera to use.
   SceneNodeCamera* sceneNodeCamera;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__CAMERACONTROLLERSCENENODECAMERA_H__
