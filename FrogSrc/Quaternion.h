#ifndef __FROG__QUATERNION_H__
#define __FROG__QUATERNION_H__

#include "FrogMemory.h"
#include "Point3.h"

namespace Webfoot {

class Matrix43;

//==============================================================================

/// Quaternions can be used to represent and manipulate rotations.
/// Much like transform matrices, rotation quaternions can be multiplied to
/// apply a series of rotations.  They must be applied from right to left.
/// For example, if you wanted a matrix that would rotate 90 degrees about x and
/// then 45 degrees about y, you could do something like...
///    Quaternion rotation90x;
///    rotation90x.RotationSet(90.0f, Point3F::Create(1.0f, 0.0f, 0.0f));
///    Quaternion rotation45y;
///    rotation45y.RotationSet(45.0f, Point3F::Create(0.0f, 1.0f, 0.0f));
///    Quaternion orientation = rotation45y * rotation90x;
///    Matrix43 transform = Matrix43::Create(orientation);
class Quaternion
{
public:
   float w;
   float x;
   float y;
   float z;

   /// Return an identity quaternion.
   static Quaternion Create() { Quaternion q; q.Set(1.0f, 0.0f, 0.0f, 0.0f); return q; }
   static Quaternion Create(const float &iw, const float &ix, const float &iy, const float &iz) throw()  { Quaternion q; q.Set(iw, ix, iy, iz); return q; }
   static Quaternion Create(float angle, const Point3<float>& axis) { Quaternion q; q.RotationSet(angle, axis); return q; }
   static Quaternion Create(const Point3F& directionBefore, const Point3F& directionAfter) { Quaternion q; q.RotationSetByDirectionDifference(directionBefore, directionAfter); return q; }
   static Quaternion Create(float i[4]) throw() { Quaternion q; q.Set(i); return q; }
   static Quaternion Create(const Matrix43& m) throw();

   void Set(float iw, float ix, float iy, float iz) throw() { w = iw; x = ix; y = iy; z = iz; }
   void Set(const float &iw, const Point3<float>& iv) throw() { w = iw; x = iv.x; y = iv.y; z = iv.z; }
   void Set(float i[4]) throw() { w = i[0]; x = i[1]; y = i[2]; z = i[3]; }

   /// Set this to represent a rotation of the given angle (in degrees)
   /// about the given normalized axis.  The rotation will be clockwise
   /// if looking in the direction of the axis.
   void RotationSet(float angle, const Point3<float>& axis);

   /// Set this to represent the given Euler angles applied in the order X, Y,
   /// Z.
   void RotationEulerXYZSet(const Point3F& _eulerAngles);
   /// Assuming this is an orthonormal rotation matrix (no scaling), return
   /// the Euler angles for this rotation assuming they are applied in the
   /// order X, Y, Z.  This is susceptible to gimbal lock when the Y angle is
   /// near +/- 90 degrees.
   Point3F RotationEulerXYZGet() const;

   /// Set this to represent the given change in direction.  This assumes both
   /// vectors are normalized.
   void RotationSetByDirectionDifference(const Point3F& directionBefore, const Point3F& directionAfter);

   Quaternion operator+() const throw() { return *this; }
   Quaternion operator-() const throw() { return Create(-w, -x, -y, -z); }

   Quaternion operator+(const Quaternion &q) const throw() { return Create(w+q.w, x+q.x, y+q.y, z+q.z); }
   Quaternion operator-(const Quaternion &q) const throw() { return Create(w-q.w, x-q.x, y-q.y, z-q.z); }
   Quaternion operator*(const Quaternion &q) const throw();
   Quaternion operator/(const Quaternion &q) const throw() { return *this * Inverse(q); }

   Quaternion& operator+=(const Quaternion &q) throw() { w+=q.w; x+=q.x; y+=q.y; z+=q.z; return *this; }
   Quaternion& operator-=(const Quaternion &q) throw() { w-=q.w; x-=q.x; y-=q.y; z-=q.z; return *this; }
   Quaternion& operator*=(const Quaternion &q) throw() { *this = *this * q; return *this; }
   Quaternion& operator/=(const Quaternion &q) throw() { *this = *this * Inverse(q); return *this; }

   // Scale operators
   Quaternion operator*(float s) const throw() { return Create(w*s, x*s, y*s, z*s); }
   Quaternion operator/(float s) const throw() { return Create(w/s, x/s, y/s, z/s); }
   friend Quaternion operator*(float s, const Quaternion &q) throw() { return Create(s*q.w, s*q.x, s*q.y, s*q.z); }
   void operator*=(float s) throw() { w *= s; x *= s; y *= s; z *= s; }
   void operator/=(float s) throw() { w /= s; x /= s; y /= s; z /= s; }

   // Dot product
   float operator%(const Quaternion &q) const throw() { return w*q.w + x*q.x + y*q.y + z*q.z; }

   // Array access operators.
   const float& operator[](int n) const throw() { return (&w)[n]; }
   float& operator[](int n) throw() { return (&w)[n]; }

   // Comparison operators
   bool operator==(const Quaternion &q) const throw() { return w==q.w && x==q.x && y==q.y && z==q.z; }
   bool operator!=(const Quaternion &q) const throw() { return w!=q.w || x!=q.x || y!=q.y || z!=q.z; }

   friend Quaternion Conjugate(const Quaternion &q) throw() { return Create(q.w, -q.x, -q.y, -q.z); }

   // Note - this only works with unit quaternions!
   friend Quaternion Inverse(const Quaternion &q) throw() { return Create(q.w, -q.x, -q.y, -q.z); }

   friend Quaternion Normalize(const Quaternion& q);

   friend Quaternion pow(const Quaternion& q, unsigned n)
   {
      Quaternion r = Create(1, 0, 0, 0);
      Quaternion k = q;

      while(n)
      {
         if(n & 1) r *= k;
         k *= k;
         n >>= 1;
      }
      return r;
   }
};

//==============================================================================

} // namespace Webfoot

#endif //#ifndef __FROG__QUATERNION_H__