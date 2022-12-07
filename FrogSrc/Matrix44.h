#ifndef __FROG__MATRIX44_H__
#define __FROG__MATRIX44_H__

#include "FrogMemory.h"
#include "Point4.h"
#include "FrogMath.h"
#include "Debug.h"
#include "Matrix43.h"
#include "Utility.h"

namespace Webfoot {

//==============================================================================

/// 4x4 column-major matrix.
class Matrix44
{
public:
   typedef float T;

   Point4<T> m[4];

   /// Return a matrix with the given elements in column-major order.
   static Matrix44 Create(T m11, T m12, T m13, T m14,
                          T m21, T m22, T m23, T m24,
                          T m31, T m32, T m33, T m34,
                          T m41, T m42, T m43, T m44)
   {
      Matrix44 m;

      m[0][0] = m11; m[0][1] = m12; m[0][2] = m13; m[0][3] = m14;
      m[1][0] = m21; m[1][1] = m22; m[1][2] = m23; m[1][3] = m24;
      m[2][0] = m31; m[2][1] = m32; m[2][2] = m33; m[2][3] = m34;
      m[3][0] = m41; m[3][1] = m42; m[3][2] = m43; m[3][3] = m44;

      return m;
   }

   /// Convert the given Matrix43 to a Matrix44.
   static Matrix44 Create(const Matrix43& other)
   {
      Matrix44 m;

      m.m[0].x = other.m[0].x; m.m[1].x = other.m[1].x; m.m[2].x = other.m[2].x; m.m[3].x = other.m[3].x;
      m.m[0].y = other.m[0].y; m.m[1].y = other.m[1].y; m.m[2].y = other.m[2].y; m.m[3].y = other.m[3].y;
      m.m[0].z = other.m[0].z; m.m[1].z = other.m[1].z; m.m[2].z = other.m[2].z; m.m[3].z = other.m[3].z;
      m.m[0].w = 0.0f;         m.m[1].w = 0.0f;         m.m[2].w = 0.0f;         m.m[3].w = 1.0f;

      return m;
   }

   /// Make this matrix an identity matrix.
   void IdentitySet()
   {
      m[0].Set(1, 0, 0, 0);
      m[1].Set(0, 1, 0, 0);
      m[2].Set(0, 0, 1, 0);
      m[3].Set(0, 0, 0, 1);
   }

   Point4<T>& operator[](int n) { return m[n]; }
   const Point4<T>& operator[](int n) const { return m[n]; }

   Point4<T> RowGet(int n) const { return Point4<T>::Create(m[0][n], m[1][n], m[2][n], m[3][n]); }

   /// Multiply the two matrices.
   Matrix44 operator*(const Matrix44 &b) const
   {
	   Matrix44 r;

	   r.m[0].Set(m[0].x*b.m[0].x + m[1].x*b.m[0].y + m[2].x*b.m[0].z + m[3].x*b.m[0].w,
			    m[0].y*b.m[0].x + m[1].y*b.m[0].y + m[2].y*b.m[0].z + m[3].y*b.m[0].w,
			    m[0].z*b.m[0].x + m[1].z*b.m[0].y + m[2].z*b.m[0].z + m[3].z*b.m[0].w,
             m[0].w*b.m[0].x + m[1].w*b.m[0].y + m[2].w*b.m[0].z + m[3].w*b.m[0].w);
	   r.m[1].Set(m[0].x*b.m[1].x + m[1].x*b.m[1].y + m[2].x*b.m[1].z + m[3].x*b.m[1].w,
			    m[0].y*b.m[1].x + m[1].y*b.m[1].y + m[2].y*b.m[1].z + m[3].y*b.m[1].w,
			    m[0].z*b.m[1].x + m[1].z*b.m[1].y + m[2].z*b.m[1].z + m[3].z*b.m[1].w,
             m[0].w*b.m[1].x + m[1].w*b.m[1].y + m[2].w*b.m[1].z + m[3].w*b.m[1].w);
	   r.m[2].Set(m[0].x*b.m[2].x + m[1].x*b.m[2].y + m[2].x*b.m[2].z + m[3].x*b.m[2].w,
			    m[0].y*b.m[2].x + m[1].y*b.m[2].y + m[2].y*b.m[2].z + m[3].y*b.m[2].w,
			    m[0].z*b.m[2].x + m[1].z*b.m[2].y + m[2].z*b.m[2].z + m[3].z*b.m[2].w,
             m[0].w*b.m[2].x + m[1].w*b.m[2].y + m[2].w*b.m[2].z + m[3].w*b.m[2].w);
      r.m[3].Set(m[0].x*b.m[3].x + m[1].x*b.m[3].y + m[2].x*b.m[3].z + m[3].x*b.m[3].w,
			    m[0].y*b.m[3].x + m[1].y*b.m[3].y + m[2].y*b.m[3].z + m[3].y*b.m[3].w,
			    m[0].z*b.m[3].x + m[1].z*b.m[3].y + m[2].z*b.m[3].z + m[3].z*b.m[3].w,
             m[0].w*b.m[3].x + m[1].w*b.m[3].y + m[2].w*b.m[3].z + m[3].w*b.m[3].w);

	   return r;
   }

   /// Multiply this matrix by the given column vector.
   Point4<T> operator*(const Point4<T> &p) const
   {
      return Point4<T>::Create(static_cast<T>(p.x*m[0][0] + p.y*m[1][0] + p.z*m[2][0] + p.w*m[3][0]),
         static_cast<T>(p.x*m[0][1] + p.y*m[1][1] + p.z*m[2][1] + p.w*m[3][1]),
         static_cast<T>(p.x*m[0][2] + p.y*m[1][2] + p.z*m[2][2] + p.w*m[3][2]),
         static_cast<T>(p.x*m[0][3] + p.y*m[1][3] + p.z*m[2][3] + p.w*m[3][3]));
   }

   /// Make this matrix into its transpose.
   void Transpose()
   {
      Swap(m[0][1], m[1][0]);
      Swap(m[0][2], m[2][0]);
      Swap(m[1][2], m[2][1]);
      Swap(m[0][3], m[3][0]);
      Swap(m[1][3], m[3][1]);
      Swap(m[2][3], m[3][2]);
   }

   /// Set this matrix to be a perspective projection matrix.
   /// 'verticalFieldOfView' should be given in degrees.  This assumes that
   /// clip coordinates range from -1 to 1 in all cardinal directions and that
   /// column vectors will be used for vertex positions.
   void PerspectiveSet(float verticalFieldOfView, float aspectRatio,
      float nearDistance, float farDistance)
   {
      float f = 1.0f / tan(DegreesToRadians(verticalFieldOfView) / 2.0f);
      m[0].Set(f / aspectRatio, 0.0f, 0.0f, 0.0f);
      m[1].Set(0.0f, f, 0.0f, 0.0f);
      m[2].Set(0.0f, 0.0f, (farDistance + nearDistance)/(nearDistance - farDistance), -1.0f);
      m[3].Set(0.0f, 0.0f, (2.0f * farDistance * nearDistance)/(nearDistance - farDistance), 0.0f);
   }

   /// Set this matrix to be an orthographic projection matrix.
   /// This assumes that clip coordinates range from -1 to 1 in all cardinal
   /// directions and that column vectors will be used for vertex positions.
   void OrthographicSet(float left, float right, float bottom, float top, float _near, float _far)
   {
      float rightMinusLeft = right - left;
      float topMinusBottom = top - bottom;
      float farMinusNear = _far - _near;
      assert(rightMinusLeft);
      assert(topMinusBottom);
      assert(farMinusNear);
      m[0].Set(2.0f / rightMinusLeft, 0.0f, 0.0f, 0.0f);
      m[1].Set(0.0f, 2.0f / topMinusBottom, 0.0f, 0.0f);
      m[2].Set(0.0f, 0.0f, -2.0f / farMinusNear, 0.0f);
      m[3].Set(-(right + left)/rightMinusLeft, -(top + bottom)/topMinusBottom, -(_far + _near)/farMinusNear, 1.0f);
   }

   //void operator<<=(int a) { m[0] <<= a; m[1] <<= a; m[2] <<= a; m[3] <<= a; }
   //void operator>>=(int a) { m[0] >>= a; m[1] >>= a; m[2] >>= a; m[3] >>= a; }
};

//==============================================================================

} // namespace Webfoot

#endif //#ifdef __FROG__MATRIX44_H__
