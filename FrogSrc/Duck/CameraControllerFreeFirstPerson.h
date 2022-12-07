#ifndef __FROG__DUCK__CAMERACONTROLLERFREEFIRSTPERSON_H__
#define __FROG__DUCK__CAMERACONTROLLERFREEFIRSTPERSON_H__

#include "FrogMemory.h"
#include "Duck/CameraController.h"

namespace Webfoot {
namespace Duck {

class SceneInstance;

//==============================================================================

/// Camera that uses the mouse and keyboard to allow the user to move around
/// freely.  The orientation is restricted to azimuth and elevation.
class CameraControllerFreeFirstPerson : public CameraController
{
public:
   virtual void Init(SceneInstance* _sceneInstance);
   virtual void Update(unsigned int dt);

   virtual Matrix43 ViewMatrixCenterGet();
   virtual Matrix43 CameraTransformCenterGet();

   /// Set the very fast speed at which the position changes in world units per second.
   void TranslationSpeedVeryFastSet(float _veryFastTranslationSpeed) { veryFastTranslationSpeed = _veryFastTranslationSpeed; }
   /// Return the very fast speed at which the position changes in world units per second.
   float TranslationSpeedVeryFastGet() { return veryFastTranslationSpeed; }

   /// Set the fast speed at which the position changes in world units per second.
   void TranslationSpeedFastSet(float _fastTranslationSpeed) { fastTranslationSpeed = _fastTranslationSpeed; }
   /// Return the fast speed at which the position changes in world units per second.
   float TranslationSpeedFastGet() { return fastTranslationSpeed; }

   /// Set the speed at which the position changes in world units per second.
   void TranslationSpeedNormalSet(float _normalTranslationSpeed) { normalTranslationSpeed = _normalTranslationSpeed; }
   /// Return the speed at which the position changes in world units per second.
   float TranslationSpeedNormalGet() { return normalTranslationSpeed; }

   /// Set the slow speed at which the position changes in world units per second.
   void TranslationSpeedSlowSet(float _slowTranslationSpeed) { slowTranslationSpeed = _slowTranslationSpeed; }
   /// Return the slow speed at which the position changes in world units per second.
   float TranslationSpeedSlowGet() { return slowTranslationSpeed; }

   /// Set the multiplier for the translation speed.
   void TranslationSpeedCoefficientSet(float _translationSpeedCoefficient);
   /// Return the multiplier for the translation speed.
   float TranslationSpeedCoefficientGet() { return translationSpeedCoefficient; }

   /// Set the minimum multiplier for the translation speed.
   void TranslationSpeedCoefficientMinSet(float _translationSpeedCoefficientMin);
   /// Return the minimum multiplier for the translation speed.
   float TranslationSpeedCoefficientMinGet() { return translationSpeedCoefficientMin; }

   /// Set the maximum multiplier for the translation speed.
   void TranslationSpeedCoefficientMaxSet(float _translationSpeedCoefficientMax);
   /// Return the maximum multiplier for the translation speed.
   float TranslationSpeedCoefficientMaxGet() { return translationSpeedCoefficientMax; }

   /// Set the coefficient that controls how fast the view angle changes as the mouse moves.
   void MouseRotationSensitivitySet(float _mouseRotationSensitivity) { mouseRotationSensitivity = _mouseRotationSensitivity; }
   /// Return the coefficient that controls how fast the view angle changes as the mouse moves.
   float MouseRotationSensitivityGet() { return mouseRotationSensitivity; }

   /// Set the coefficient that controls how fast the camera translation
   /// coefficient changes as the mouse's scroll wheel is turned.
   void MouseScrollSensitivitySet(float _mouseScrollSensitivity) { mouseScrollSensitivity = _mouseScrollSensitivity; }
   /// Return the coefficient that controls how fast the camera translation
   /// coefficient changes as the mouse's scroll wheel is turned.
   float MouseScrollSensitivityGet() { return mouseScrollSensitivity; }

   /// Set the position of the camera.
   void PositionSet(const Point3F& _position) { position = _position; }
   /// Return the position of the camera.
   Point3F PositionGet() { return position; }

   /// Set the angle of yaw of the camera.
   void AzimuthAngleSet(float _pitchAngle) { azimuthAngle = _pitchAngle; }
   /// Return the angle of yaw of the camera.
   float AzimuthAngleGet() { return azimuthAngle; }

   /// Set the angle of pitch of the camera.
   void ElevationAngleSet(float _pitchAngle) { elevationAngle = _pitchAngle; }
   /// Return the angle of pitch of the camera.
   float ElevationAngleGet() { return elevationAngle; }

   /// Create and return JSON specifications for the current camera transform.
   JSONValue* TransformJSONGet();
   /// Set the camera transform based on the given JSON specifications.
   void TransformJSONSet(JSONValue* transformSpecs);

   typedef CameraController Inherited;

private:
   /// Angle of rotation about the vertical axis, in degrees.
   float azimuthAngle;
   /// Angle of rotation about the left-right axis, after azimuth is applied, in degrees.
   float elevationAngle;
   /// Position of the camera in world space.
   Point3F position;
   /// Very fast speed at which the camera's position changes in world units per second.
   float veryFastTranslationSpeed;
   /// Fast speed at which the camera's position changes in world units per second.
   float fastTranslationSpeed;
   /// Speed at which the camera's position changes in world units per second.
   float normalTranslationSpeed;
   /// Slow speed at which the camera's position changes in world units per second.
   float slowTranslationSpeed;
   /// Multiplier for the translation speed.
   float translationSpeedCoefficient;
   /// Minimum multiplier for the translation speed.
   float translationSpeedCoefficientMin;
   /// Maximum multiplier for the translation speed.
   float translationSpeedCoefficientMax;
   /// Coefficient that controls how fast the view angle changes as the mouse moves.
   float mouseRotationSensitivity;
   /// Coefficient that controls how fast the camera translation coefficient
   /// changes as the mouse's scroll wheel is turned.
   float mouseScrollSensitivity;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__CAMERACONTROLLERFREEFIRSTPERSON_H__
