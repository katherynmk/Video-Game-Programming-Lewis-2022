#ifndef __FROG__CIRCLE3_H__
#define __FROG__CIRCLE3_H__

#include "FrogMemory.h"
#include "Point3.h"

namespace Webfoot {

class Matrix43;

//==============================================================================

/// Circle3 is a struct for a point paired with a radius in 3D space - ie. a Sphere
struct Circle3
{
public:
   /// center position
   Point3F center;
   /// radius
   float radius;

   Circle3() {}
   Circle3(const Point3F& _center, float _radius) : center(_center), radius(_radius) { }

   /// Multiply the matrix by the given sphere.
   friend Circle3 operator*(const Matrix43 &m, const Circle3& c);

   // Returns a new sphere that encompasses both spheres
   Circle3 operator+(const Circle3& a) const;

   /// Return true if this sphere contains the given point.
   bool ContainsCheck(const Point3F& p) const;

   /// Returns the union of this sphere with another.
   Circle3 Union(const Circle3& b) const;

   static Circle3 emptyInstance;
};

Circle3 operator*(const Matrix43 &m, const Circle3& c);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__CIRCLE3_H__
