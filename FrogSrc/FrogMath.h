#ifndef __FROG__FROGMATH_H__
#define __FROG__FROGMATH_H__

#include "FrogMemory.h"
#include "Box2.h"
#include "Plane.h"
#include "Sphere.h"
#include "Ray3.h"
#include "Quaternion.h"
#include <cmath>
#include <limits.h>

namespace Webfoot {

const float PI = 3.14159265f;
const float DEGREES_PER_CIRCLE = 360.0f;
const float RADIANS_PER_CIRCLE = PI * 2.0f;

//=============================================================================

/// Class for basic math operations.
class FrogMath
{
public:
   /// First half of the seed for random number generation.
   static unsigned int Seed1;
   /// Second half of the seed for random number generation.
   static unsigned int Seed2;

   /// Return a pseudo-random integer
   static unsigned int Random();

   /// Return a pseudo-random integer between 0 and Range-1.  Range can be a maximum of 65536
   static unsigned int Random(unsigned Range);

   /// Return a pseudo-random float between 0.0f and 1.0f.
   static float RandomF();

   /// Return a value that is randomly +/- 'deviation' from the given 'baseValue'.
   static inline float RandomDeviationF(float baseValue, float deviation)
   {
      return (baseValue - deviation) + (FrogMath::RandomF() * 2.0f * deviation);
   }

   /// Return an approximation of the sin of the given angle in degrees.
   static float Sin(float angle);
   /// Return an approximation of the cos of the given angle in degrees.
   static inline float Cos(float angle) { return Sin(angle + 90.0f); }

   /// Return true if the given spheres overlap.
   static bool Intersect(const Sphere& sphere1, const Sphere& sphere2);
   /// If 'intersectDistanceMax' is specified and the distance to the
   /// intersection is greater than the value it provides, return false.
   /// Otherwise, if the given ray intersects the given sphere, return true,
   /// set '_intersectDistance' set to the distance to the intersection, and
   /// set '_intersectPosition' to the point at which they intersect.  If the
   /// ray would need to go backwards to intersect the sphere, return false.
   static bool Intersect(const Ray3& ray, const Sphere& sphere,
      float* intersectDistanceMax = NULL, float* intersectDistance = NULL,
      Point3F* intersectPosition = NULL);
   /// If 'intersectDistanceMax' is specified and the distance to the
   /// intersection is greater than the value it provides, return false.
   /// Otherwise, if the ray intersects the given plane, return true, set
   /// '_intersectDistance' set to the distance to the intersection, and set
   /// '_intersectPosition' to the point at which they intersect.  If the ray
   /// intersects the back face of the plane, return false.  If the ray would
   /// need to go backwards to intersect the plane, return false.
   static bool Intersect(const Ray3& ray, const Plane& plane,
      float* intersectDistanceMax = NULL, float* _intersectDistance = NULL,
      Point3F* _intersectPosition = NULL);
   /// If 'intersectDistanceMax' is specified and the distance to the
   /// intersection is greater than the value it provides, return false.
   /// Otherwise, if the ray intersects the given triangle, return true, set
   /// '_intersectPosition' to the point at which they intersect, set
   /// '_intersectDistance' to the distance to the intersection, and set
   /// '_intersectNormal' to the normal vector of the triangle.  If the ray
   /// intersects the back face of the triangle, return false.  If the ray
   /// would need to go backwards to intersect the triangle, return false.
   static bool Intersect(const Ray3& ray, const Point3F& v0, const Point3F& v1, const Point3F& v2,
      float* intersectDistanceMax = NULL, float* _intersectDistance = NULL,
      Point3F* _intersectPosition = NULL, Point3F* _intersectNormal = NULL);
};

//=============================================================================

/// Non-static version of the random number generator in FrogMath.  The
/// implementation in FrogMath should be used in general, but this can be
/// useful when you want an independent random number generator, like on a
/// different thread or for easier determinism.
class RandomNumberGenerator
{
public:
   /// First half of the seed for random number generation.
   unsigned int Seed1;
   /// Second half of the seed for random number generation.
   unsigned int Seed2;

   /// Return a pseudo-random integer
   unsigned int Random();

   /// Return a pseudo-random integer between 0 and Range-1.  Range can be a maximum of 65536
   unsigned int Random(unsigned Range);

   /// Return a pseudo-random float between 0.0f and 1.0f.
   float RandomF();

   /// Return a value that is randomly +/- 'deviation' from the given 'baseValue'.
   inline float RandomDeviationF(float baseValue, float deviation)
   {
      return (baseValue - deviation) + (RandomF() * 2.0f * deviation);
   }
};

//=============================================================================

/// Clamps a value to a specific range. Takes in a value to check, a minimum,
/// and a maximum. Returns the clamped value.
template <typename T>
T Clamp(T value, T min, T max)
{
   if(value < min)
      return min;
   else if(value > max)
      return max;
   else
      return value;
}

//=============================================================================

/// Return 'value' rounded up to the nearest multiple of 'denominator'.
/// 'denominator' must be positive.  Both must have the same integral type.
template <typename T>
T RoundUpToMultiple(T value, T denominator)
{
   // Checking (remainder < 0) implicitly rather than explicitly is to suppress
   // a warning on the Wii U for when T is unsigned and checking if it's
   // negative is unnecessary.  Attempting to suppress the warning with a
   // #pragma didn't work.

   T remainder = value % denominator;
   if(remainder > 0)
      return value + (denominator - remainder);
   else if(remainder == 0)
      return value;
   else //if(remainder < 0)
      return value - remainder;
}

//-----------------------------------------------------------------------------

/// Return 'value' rounded down to the nearest multiple of 'denominator'.
/// 'denominator' must be positive.  Both must have the same integral type.
template <typename T>
T RoundDownToMultiple(T value, T denominator)
{
   // Checking (remainder < 0) implicitly rather than explicitly is to suppress
   // a warning on the Wii U for when T is unsigned and checking if it's
   // negative is unnecessary.  Attempting to suppress the warning with a
   // #pragma didn't work.
   
   T remainder = value % denominator;
   if(remainder > 0)
      return value - remainder;
   else if(remainder == 0)
      return value;
   else //if(remainder < 0)
      return value - (denominator + remainder);
}

//=============================================================================

/// Given a number of degrees, return the equivalent in radians.
float DegreesToRadians(float degrees);

//-----------------------------------------------------------------------------

/// Given a number of radians, return the equivalent in degrees.
float RadiansToDegrees(float radians);

//-----------------------------------------------------------------------------

/// Return log base 2 of x.
inline float Log2(float x) { return log(x) * 1.442695f; }

//-----------------------------------------------------------------------------

/// Return x rounded to the nearest integer.
inline float Round(float x) { return floor(x + 0.5f); }

//=============================================================================

/// Linear interpolation
template<typename T>
inline T Lerp(const T& valueBegin, const T& valueEnd, float t, bool clampT = true)
{
   if(clampT)
   {
      if(t > 1.0f)
         t = 1.0f;
      else if(t < 0.0f)
         t = 0.0f;
   }
   return (valueBegin * (1.0f - t)) + (valueEnd * t);
}

//-----------------------------------------------------------------------------

/// Linear interpolation
template<>
inline uchar Lerp<uchar>(const uchar& valueBegin, const uchar& valueEnd, float t, bool clampT)
{
   if(clampT)
   {
      if(t > 1.0f)
         t = 1.0f;
      else if(t < 0.0f)
         t = 0.0f;
   }
   return (uchar)(((float)valueBegin * (1.0f - t)) + ((float)valueEnd * t));
}

//-----------------------------------------------------------------------------

/// Linear interpolation
template<>
inline int Lerp<int>(const int& valueBegin, const int& valueEnd, float t, bool clampT)
{
   if(clampT)
   {
      if(t > 1.0f)
         t = 1.0f;
      else if(t < 0.0f)
         t = 0.0f;
   }
   return (int)(((float)valueBegin * (1.0f - t)) + ((float)valueEnd * t));
}

//-----------------------------------------------------------------------------

/// Linear interpolation
template<>
inline unsigned int Lerp<unsigned int>(const unsigned int& valueBegin, const unsigned int& valueEnd, float t, bool clampT)
{
   if(clampT)
   {
      if(t > 1.0f)
         t = 1.0f;
      else if(t < 0.0f)
         t = 0.0f;
   }
   return (unsigned int)(((float)valueBegin * (1.0f - t)) + ((float)valueEnd * t));
}

//-----------------------------------------------------------------------------

/// Linear interpolation
template<>
inline Quaternion Lerp<Quaternion>(const Quaternion& valueBegin, const Quaternion& valueEnd, float t, bool clampT)
{
   if(clampT)
   {
      if(t > 1.0f)
         t = 1.0f;
      else if(t < 0.0f)
         t = 0.0f;
   }
   // With quaternions, look out for lerping in a way that would add up to
   // something approaching zero.
   Quaternion _valueBegin = valueBegin;
   if(((valueBegin.w * valueEnd.w) + (valueBegin.x * valueEnd.x) + (valueBegin.y * valueEnd.y) + (valueBegin.z * valueEnd.z)) < 0.0f)
      _valueBegin *= -1.0f;
   return Normalize((_valueBegin * (1.0f - t)) + (valueEnd * t));
}

//=============================================================================

/// Bezier quadratic interpolation
template<typename T>
inline T BezierQuadraticInterpolate(const T& valueBegin, const T& valueMiddle, const T& valueEnd, float t)
{
   if(t > 1.0f)
      t = 1.0f;
   else if(t < 0.0f)
      t = 0.0f;
   float mt = 1 - t;
   return mt * mt * valueBegin + 2 * mt * t * valueMiddle + t * t * valueEnd;
}

//-----------------------------------------------------------------------------

/// Bezier quadratic interpolation
template<>
inline uchar BezierQuadraticInterpolate<uchar>(const uchar& valueBegin, const uchar& valueMiddle, const uchar& valueEnd, float t)
{
   if(t > 1.0f)
      t = 1.0f;
   else if(t < 0.0f)
      t = 0.0f;
   float mt = 1 - t;
   return (uchar)(mt * mt * (float)valueBegin + 2 * mt * t * (float)valueMiddle + t * t * (float)valueEnd);
}

//-----------------------------------------------------------------------------

/// Bezier quadratic interpolation
template<>
inline int BezierQuadraticInterpolate<int>(const int& valueBegin, const int& valueMiddle, const int& valueEnd, float t)
{
   if(t > 1.0f)
      t = 1.0f;
   else if(t < 0.0f)
      t = 0.0f;
   float mt = 1 - t;
   return (int)(mt * mt * (float)valueBegin + 2 * mt * t * (float)valueMiddle + t * t * (float)valueEnd);
}

//-----------------------------------------------------------------------------

/// Bezier quadratic interpolation
template<>
inline unsigned int BezierQuadraticInterpolate<unsigned int>(const unsigned int& valueBegin, const unsigned int& valueMiddle, const unsigned int& valueEnd, float t)
{
   if(t > 1.0f)
      t = 1.0f;
   else if(t < 0.0f)
      t = 0.0f;
   float mt = 1 - t;
   return (unsigned int)(mt * mt * (float)valueBegin + 2 * mt * t * (float)valueMiddle + t * t * (float)valueEnd);
}

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROGMATH_H__
