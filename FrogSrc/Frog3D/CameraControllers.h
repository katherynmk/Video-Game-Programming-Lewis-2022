#ifndef __FROG__FROG3D__CAMERACONTROLLERS_H__
#define __FROG__FROG3D__CAMERACONTROLLERS_H__

#include "FrogMemory.h"

namespace Webfoot {

struct RenderData;
class SceneNode;
class TransformNode;
class JoystickWidget;

//=============================================================================

/// Class to control the camera.
class CameraController
{
public:
   virtual ~CameraController() { }
   virtual void Deinit() {}

   virtual void Update(RenderData* renderData) = 0;   
   virtual void Release() { Deinit(); frog_delete this; }

   /// Make 'cameraMatrix' a camera matrix that points from the given position
   /// at the given target.  If it is not possible to build the whole matrix
   /// using the given vectors, return false and only update the translation
   /// component using the current cameraMatrix value for orientation.
   /// Otherwise, return true.
   static bool MatrixLookAtGet(Matrix43& cameraMatrix, const Point3F& cameraPosition,
      const Point3F& targetPosition, const Point3F& upVector = Point3F::Create(0.0f, 0.0f, 1.0f));

   /// Make 'cameraMatrix' a camera matrix that gets its position and rotation
   /// from the given 'worldTransform' matrix.
   static void WorldTransformToCameraMatrix(Matrix43& cameraMatrix, const Matrix43& worldTransform);
};

//=============================================================================

class EmptyCameraController : public CameraController
{
public:  
   virtual void Update(RenderData*) { }
   virtual void Release() { }
   
   static EmptyCameraController instance;
};

//=============================================================================

/// Use the given scene node for the camera's perspective.  This assumes the
/// given object is a CameraSceneNode, because the transform of a camera uses
/// x for right, y for up, and z for backward.
class CameraController_AttachToSceneNode : public CameraController
{
public:  
   CameraController_AttachToSceneNode();

   void Init(SceneNode* sceneNode);
   void Init(TransformNode* transformNode);
   virtual void Deinit();

   virtual void Update(RenderData* renderData);
   
private:
   TransformNode* node;
};

//=============================================================================

/// Camera that uses the mouse and keyboard to allow the user to move around
/// freely.  The orientation is restricted to azimuth and elevation.
class CameraController_FreeFirstPerson : public CameraController
{
public:  
   void Init();

   virtual void Update(RenderData* renderData);

   /// Set the speed at which the position changes in world units per second.
   void TransationSpeedNormalSet(float _normalTranslationSpeed) { normalTranslationSpeed = _normalTranslationSpeed; }
   /// Return the speed at which the position changes in world units per second.
   float TransationSpeedNormalGet() { return normalTranslationSpeed; }

   /// Set the fast speed at which the position changes in world units per second.
   void TransationSpeedFastSet(float _fastTranslationSpeed) { fastTranslationSpeed = _fastTranslationSpeed; }
   /// Return the fast speed at which the position changes in world units per second.
   float TransationSpeedFastGet() { return fastTranslationSpeed; }

   /// Set the slow speed at which the position changes in world units per second.
   void TransationSpeedSlowSet(float _slowTranslationSpeed) { slowTranslationSpeed = _slowTranslationSpeed; }
   /// Return the slow speed at which the position changes in world units per second.
   float TransationSpeedSlowGet() { return slowTranslationSpeed; }

   /// Set the coefficient that controls how fast the view angle changes as the mouse moves.
   void MouseSensitivitySet(float _mouseSensitivity) { mouseSensitivity = _mouseSensitivity; }
   /// Return the coefficient that controls how fast the view angle changes as the mouse moves.
   float MouseSensitivityGet() { return mouseSensitivity; }

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
   
private:
   /// Angle of rotation about the vertical axis, in degrees.
   float azimuthAngle;
   /// Angle of rotation about the left-right axis, after azimuth is applied, in degrees.
   float elevationAngle;
   /// Position of the camera in world space.
   Point3F position;
   /// Speed at which the camera's position changes in world units per second.
   float normalTranslationSpeed;
   /// Fast speed at which the camera's position changes in world units per second.
   float fastTranslationSpeed;
   /// Slow speed at which the camera's position changes in world units per second.
   float slowTranslationSpeed;
   /// Coefficient that controls how fast the view angle changes as the mouse moves.
   float mouseSensitivity;
};

//=============================================================================

/// Camera that uses the mouse to allow the user to move in a sphere around a
/// target node.  The orientation is restricted to azimuth and elevation.
class CameraController_FreeSphere : public CameraController
{
public:  
   CameraController_FreeSphere();

   void Init();
   virtual void Deinit();

   virtual void Update(RenderData* renderData);

   /// Set the coefficient that controls how fast the view angle changes as the mouse moves.
   void MouseSensitivitySet(float _mouseSensitivity) { mouseSensitivity = _mouseSensitivity; }
   /// Return the coefficient that controls how fast the view angle changes as the mouse moves.
   float MouseSensitivityGet() { return mouseSensitivity; }

   /// Set the coefficient that controls how fast the distance between the
   /// camera and the target changes as the mouse's scroll wheel is turned.
   void MouseScrollSensitivitySet(float _mouseScrollSensitivity) { mouseScrollSensitivity = _mouseScrollSensitivity; }
   /// Return the coefficient that controls how fast the distance between the
   /// camera and the target changes as the mouse's scroll wheel is turned.
   float MouseScrollSensitivityGet() { return mouseScrollSensitivity; }

   /// Set the node to watch.
   void TargetNodeSet(SceneNode* _targetNode) { targetNode = _targetNode; }
   /// Set an offset in the local space of the target node at which the camera
   /// should point. 
   void TargetOffsetSet(const Point3F& _targetOffset) { targetOffset = _targetOffset; }
   /// Set the distance between the camera and the target in world units.
   void DistanceFromTargetSet(float _distanceFromTarget) { distanceFromTarget = _distanceFromTarget; }

   /// Set the angle of elevation in degrees.
   void ElevationAngleSet(float _elevationAngle);
   /// Set the minimum angle of elevation in degrees.
   void ElevationAngleMinSet(float _elevationMin) { elevationMin = _elevationMin; }
   /// Set the maximum angle of elevation in degrees.
   void ElevationAngleMaxSet(float _elevationMax) { elevationMax = _elevationMax; }

   /// Set the angle of azimuth.
   void AzimuthAngleSet(float _azimuthAngle) { azimuthAngle = _azimuthAngle; }

   /// Set a JoystickWidget to control the camera angle.
   void JoystickWidgetSet(JoystickWidget* _joystickWidget) { joystickWidget = _joystickWidget; }
   /// Set the coefficient to control the strength of the response to the joystick widget.
   void JoystickWidgetSensitivitySet(float _joystickWidgetSensitivity) { joystickWidgetSensitivity = _joystickWidgetSensitivity; }

   typedef CameraController Inherited;
   
private:
   /// Angle of rotation about the vertical axis, in degrees.
   float azimuthAngle;
   /// Angle of rotation about the left-right axis, after azimuth is applied, in degrees.
   float elevationAngle;
   /// Node to watch.
   SceneNode* targetNode;
   /// Offset in the local space of the target node at which the camera should
   /// point. 
   Point3F targetOffset;
   /// Set the distance between the camera and the target in world units.
   float distanceFromTarget;
   /// Minimum angle of elevation in degrees.
   float elevationMin;
   /// Maximum angle of elevation in degrees.
   float elevationMax;
   /// Coefficient that controls how fast the view angle changes as the mouse moves.
   float mouseSensitivity;
   /// Coefficient that controls how fast the distance between the
   /// camera and the target changes as the mouse's scroll wheel is turned.
   float mouseScrollSensitivity;
   /// Widget to control the camera angle.
   JoystickWidget* joystickWidget;
   /// Coefficient to control the strength of the response to the joystick widget.
   float joystickWidgetSensitivity;
};

//=============================================================================

/// Camera that uses the mouse and keyboard to allow the user to move around
/// freely.  This version has been modified to work with VR HMDs.
class CameraController_FreeFirstPersonVR : public CameraController
{
public:  
   void Init();

   virtual void Update(RenderData* renderData);

   /// Set the speed at which the position changes in world units per second.
   void TransationSpeedNormalSet(float _normalTranslationSpeed) { normalTranslationSpeed = _normalTranslationSpeed; }
   /// Return the speed at which the position changes in world units per second.
   float TransationSpeedNormalGet() { return normalTranslationSpeed; }

   /// Set the fast speed at which the position changes in world units per second.
   void TransationSpeedFastSet(float _fastTranslationSpeed) { fastTranslationSpeed = _fastTranslationSpeed; }
   /// Return the fast speed at which the position changes in world units per second.
   float TransationSpeedFastGet() { return fastTranslationSpeed; }

   /// Set the slow speed at which the position changes in world units per second.
   void TransationSpeedSlowSet(float _slowTranslationSpeed) { slowTranslationSpeed = _slowTranslationSpeed; }
   /// Return the slow speed at which the position changes in world units per second.
   float TransationSpeedSlowGet() { return slowTranslationSpeed; }

   /// Set the position of the camera.
   void PositionSet(const Point3F& _position) { position = _position; }
   /// Return the position of the camera.
   Point3F PositionGet() { return position; }

   /// Set the angle of yaw of the camera.
   void AzimuthAngleSet(float _pitchAngle) { azimuthAngle = _pitchAngle; }
   /// Return the angle of yaw of the camera.
   float AzimuthAngleGet() { return azimuthAngle; }

   /// Set the amount to rotate in degrees when a rotation button is pressed.
   void RotationStepSizeSet(float _rotationStepSize) { rotationStepSize = _rotationStepSize; }
   /// Return the amount to rotate in degrees when a rotation button is pressed.
   float RotationStepSizeGet() { return rotationStepSize; }
   
private:
   /// Angle of rotation about the vertical axis, in degrees.
   float azimuthAngle;
   /// Amount to rotate in degrees when a rotation button is pressed.
   float rotationStepSize;
   /// Position of the camera in world space.
   Point3F position;
   /// Speed at which the camera's position changes in world units per second.
   float normalTranslationSpeed;
   /// Fast speed at which the camera's position changes in world units per second.
   float fastTranslationSpeed;
   /// Slow speed at which the camera's position changes in world units per second.
   float slowTranslationSpeed;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__CAMERACONTROLLERS_H__
