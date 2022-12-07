#ifndef __FROG__DUCK__CAMERACONTROLLERFREESPHERE_H__
#define __FROG__DUCK__CAMERACONTROLLERFREESPHERE_H__

#include "FrogMemory.h"
#include "Duck/CameraController.h"

namespace Webfoot {
namespace Duck {

class SceneInstance;

//==============================================================================

/// Camera that uses the mouse to allow the user to move in a sphere around a
/// target position.  The orientation is restricted to azimuth and elevation.
class CameraControllerFreeSphere : public CameraController
{
public:
   virtual void Init(SceneInstance* _sceneInstance);
   virtual void Update(unsigned int dt);

   virtual Matrix43 ViewMatrixCenterGet();
   virtual Matrix43 CameraTransformCenterGet();

   /// Set the coefficient that controls how fast the view angle changes as the mouse moves.
   void MouseRotationSensitivitySet(float _mouseRotationSensitivity) { mouseRotationSensitivity = _mouseRotationSensitivity; }
   /// Return the coefficient that controls how fast the view angle changes as the mouse moves.
   float MouseRotationSensitivityGet() { return mouseRotationSensitivity; }

   /// Set the coefficient that controls how fast the distance between the
   /// camera and the target changes as the mouse's scroll wheel is turned.
   void MouseScrollSensitivitySet(float _mouseScrollSensitivity) { mouseScrollSensitivity = _mouseScrollSensitivity; }
   /// Return the coefficient that controls how fast the distance between the
   /// camera and the target changes as the mouse's scroll wheel is turned.
   float MouseScrollSensitivityGet() { return mouseScrollSensitivity; }

   /// Set the position at which the camera points in world units.
   void TargetPositionSet(const Point3F& _targetPosition) { targetPosition = _targetPosition; }
   /// Return the position at which the camera pointsin world units.
   Point3F TargetPositionGet() { return targetPosition; }

   /// Set the distance from the camera to the point it watches in world units.
   void DistanceFromTargetSet(float _distanceFromTarget);
   /// Return the distance from the camera to the point it watches in world units.
   float DistanceFromTargetGet() { return distanceFromTarget; }

   /// Set the angle of yaw of the camera.
   void AzimuthAngleSet(float _pitchAngle) { azimuthAngle = _pitchAngle; }
   /// Return the angle of yaw of the camera.
   float AzimuthAngleGet() { return azimuthAngle; }

   /// Set the angle of pitch of the camera.
   void ElevationAngleSet(float _pitchAngle);
   /// Set the minimum angle of elevation in degrees.
   void ElevationAngleMinSet(float _elevationMin) { elevationMin = _elevationMin; }
   /// Set the maximum angle of elevation in degrees.
   void ElevationAngleMaxSet(float _elevationMax) { elevationMax = _elevationMax; }
   /// Return the angle of pitch of the camera.
   float ElevationAngleGet() { return elevationAngle; }

   typedef CameraController Inherited;
   
private:
   /// Angle of rotation about the vertical axis, in degrees.
   float azimuthAngle;
   /// Angle of rotation about the left-right axis, after azimuth is applied, in degrees.
   float elevationAngle;
   /// Position at which the camera points in world units.
   Point3F targetPosition;
   /// Distance from the camera to the point it orbits.
   float distanceFromTarget;
   /// Minimum angle of elevation in degrees.
   float elevationMin;
   /// Maximum angle of elevation in degrees.
   float elevationMax;
   /// Coefficient that controls how fast the view angle changes as the mouse moves.
   float mouseRotationSensitivity;
   /// Coefficient that controls how fast the distance between the
   /// camera and the target changes as the mouse's scroll wheel is turned.
   float mouseScrollSensitivity;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__CAMERACONTROLLERFREESPHERE_H__
