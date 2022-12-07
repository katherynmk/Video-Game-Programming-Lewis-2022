#ifndef __FROG__DUCK__CAMERACONTROLLERSCENENODE_H__
#define __FROG__DUCK__CAMERACONTROLLERSCENENODE_H__

#include "FrogMemory.h"
#include "Duck/CameraController.h"

namespace Webfoot {
namespace Duck {

class SceneInstance;
class SceneNode;

//==============================================================================

/// Camera that follows a scene node to get its transform.  Additional position
/// and rotation offsets from the target scene node may be specified.
class CameraControllerSceneNode : public CameraController
{
public:
   CameraControllerSceneNode();

   virtual void Init(SceneInstance* _sceneInstance);
   virtual void Deinit();

   virtual Matrix43 ViewMatrixCenterGet();
   virtual Matrix43 CameraTransformCenterGet();

   /// Set the SceneNode to follow.
   void SceneNodeTargetSet(SceneNode* _sceneNodeTarget) { sceneNodeTarget = _sceneNodeTarget; }
   /// Return the SceneNode to follow.
   SceneNode* SceneNodeTargetSet() { return sceneNodeTarget; }

   /// Set the position offset from the target scene node to the camera.
   void PositionOffsetSet(const Point3F& _positionOffset) { positionOffset = _positionOffset; }
   /// Return the position offset from the target scene node to the camera.
   Point3F PositionOffsetGet() { return positionOffset; }

   /// Set the rotation offset from the target scene node to the camera.
   void RotationOffsetSet(const Quaternion& _rotationOffset) { rotationOffset = _rotationOffset; }
   /// Return the rotation offset from the target scene node to the camera.
   Quaternion RotationOffsetGet() { return rotationOffset; }

   typedef CameraController Inherited;

private:
   /// SceneNode to follow.
   SceneNode* sceneNodeTarget;
   /// Position offset from the target scene node to the camera.
   Point3F positionOffset;
   /// Rotation offset from the target scene node to the camera.
   Quaternion rotationOffset;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__CAMERACONTROLLERSCENENODE_H__
