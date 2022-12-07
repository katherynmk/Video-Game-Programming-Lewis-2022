#ifndef __WAGECAMERA_H__
#define __WAGECAMERA_H__

#include "Frog.h"

namespace Webfoot {

//==============================================================================

class WageCamera
{
public:
   WageCamera();
   ~WageCamera() {}
   
   virtual void Init();
   virtual void Deinit();

   virtual void Update(unsigned int dt);
   virtual void Draw();

   /// Move the camera to the given position and orient it to look at the target
   void LookAt(const Point3F& cameraPosition, const Point3F& targetPosition);

   /// Get the position of the camera
   Point3F PositionGet() { return position; }
   /// Set the position of the camera
   void PositionSet(const Point3F& _position) { position = _position; }

   /// Get the rotation of the camera (x, y, z in degrees)
   Point3F RotationGet() { return rotation; }
   /// Set the rotation of the camera (x, y, z in degrees)
   void RotationSet(const Point3F& _rotation) { rotation = _rotation; }

   /// Get the position of the camera
   Point3F TargetGet() { return target; }
   /// Set the position of the camera
   void TargetSet(const Point3F& _target) { target = _target; }

   static WageCamera instance;

protected:
   /// Camera (eye) position
   Point3F position;
   /// Camera rotation
   Point3F rotation;
   /// Target position
   Point3F target;
   /// Rotation quaternion (X)
   Quaternion rotationX;
   /// Rotation quaternion (Y)
   Quaternion rotationY;

private:
};

WageCamera* const theWageCamera = &WageCamera::instance;

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGECAMERA_H__
