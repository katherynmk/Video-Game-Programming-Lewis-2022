#ifndef __FROG__PLANE_H__
#define __FROG__PLANE_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Point4.h"

namespace Webfoot {

//==============================================================================

class Plane
{
public:
   /// Set the plane based on coefficients.
   void Set(const Point4F& coefficients);
   /// Set the plane based on a position on the plane and a normal vector.
   void SetByPositionAndNormal(const Point3F& _position, const Point3F& _normal);

   /// Return the distance from the plane to the given point, along the
   /// direction of the plane's normal.  Note that this would be a negative
   /// number if the point is on the back side of the plane.
   float DistanceSignedGet(const Point3F& point) const { return (point % normal) + d; }

   /// Project the given point onto the plane along the direction of the plane
   /// normal.
   Point3F Project(const Point3F& pointToProject) const;

   /// The normalized direction of the plane.
   Point3F normal;
   /// Offset from the plane to the origin.
   float d;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__PLANE_H__
