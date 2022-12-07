#ifndef __FROG__RAY3_H__
#define __FROG__RAY3_H__

#include "FrogMemory.h"
#include "Point3.h"

namespace Webfoot {

class Matrix43;

//==============================================================================

/// Ray3 is a struct for a point paired with a direction vector in 3D space.
struct Ray3
{
   /// Position of the starting point of the ray.
   Point3F p;
   /// Direction in which the ray points.  This should generally be normalized
   /// to a length of 1.
   Point3F d;
   
   Ray3() {}
   Ray3(const Point3F& _p, const Point3F& _d) : p(_p), d(_d) {}
   
   /// Multiply the matrix by the given ray.
   friend Ray3 operator*(const Matrix43 &m, const Ray3& r);
};

Ray3 operator*(const Matrix43 &m, const Ray3& r);

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__RAY3_H__
