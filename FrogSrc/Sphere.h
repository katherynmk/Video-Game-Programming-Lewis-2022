#ifndef __FROG__SPHERE_H__
#define __FROG__SPHERE_H__

#include "FrogMemory.h"
#include "Point3.h"

namespace Webfoot {

class Matrix43;

//==============================================================================

/// Sphere is a struct for a point paired with a radius in 3D space.
struct Sphere
{
public:
   /// center position
   Point3F center;
   /// radius
   float radius;

   Sphere() {}
   Sphere(const Point3F& _center, float _radius) : center(_center), radius(_radius) { }

   /// Multiply the matrix by the given sphere.
   friend Sphere operator*(const Matrix43 &m, const Sphere& c);

   // Returns a new sphere that encompasses both spheres
   Sphere operator+(const Sphere& a) const;

   /// Return true if this sphere contains the given point.
   bool ContainsCheck(const Point3F& p) const;

   /// Returns the union of this sphere with another.
   Sphere Union(const Sphere& b) const;

   static Sphere emptyInstance;
};

Sphere operator*(const Matrix43 &m, const Sphere& c);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SPHERE_H__
