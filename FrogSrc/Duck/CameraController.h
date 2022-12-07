#ifndef __FROG__DUCK__CAMERACONTROLLER_H__
#define __FROG__DUCK__CAMERACONTROLLER_H__

#include "FrogMemory.h"
#include "Matrix43.h"

namespace Webfoot {
namespace Duck {

class SceneInstance;

//==============================================================================

/// Base class for providing the view matrix for a SceneInstance.
class CameraController
{
public:
   CameraController();

   virtual void Init(SceneInstance* _sceneInstance) { sceneInstance = _sceneInstance; }
   virtual void Deinit();
   virtual void Update(unsigned int dt) { (void)dt; }

   /// Return the view matrix excluding any offset added for stereoscopy.
   virtual Matrix43 ViewMatrixCenterGet() = 0;
   /// Return the matrix for going from the local space of the camera (not to
   /// be confused with view space) into world space, excluding any offset
   /// added for stereoscopy.
   virtual Matrix43 CameraTransformCenterGet() = 0;

   /// Make 'viewMatrix' a view matrix that points from the given position
   /// at the given target.  If it is not possible to build the whole matrix
   /// using the given vectors, return false and only update the translation
   /// component using the current viewMatrix value for orientation.
   /// Otherwise, return true.
   static bool MatrixLookAtGet(Matrix43* viewMatrix, const Point3F& cameraPosition,
      const Point3F& targetPosition, const Point3F& upVector = Point3F::Create(0.0f, 0.0f, 1.0f));

   /// Make 'viewMatrix' a view matrix that gets its position and rotation
   /// from the given 'worldTransform' matrix.
   static void WorldTransformToViewMatrix(Matrix43* viewMatrix, const Matrix43& worldTransform);
   /// Make 'worldTransform' a transform matrix that gets its position and
   /// rotation from the given 'viewMatrix'.
   static void ViewMatrixToWorldTransform(Matrix43* worldTransform, const Matrix43& viewMatrix);

protected:
   /// SceneInstance for which this object controls the view matrix.
   SceneInstance* sceneInstance;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__CAMERACONTROLLER_H__
