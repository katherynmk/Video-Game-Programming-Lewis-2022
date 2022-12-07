#ifndef __FROG__MATRIX32_H__
#define __FROG__MATRIX32_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Point3.h"

namespace Webfoot {

//==============================================================================

/// Matrix32 is a 3x3 column-major matrix where the bottom row is locked to [0, 0, 1].
/// When working with vertices, they will be treated as column-vectors.
/// When building transforms, apply them from right to left.  For example, if you had
/// separate matrices for scaling, rotation, and translation, and you wanted to
/// apply them in that order, the code would look like...
///    Matrix32 transform = translation * rotation * scaling;
/// To then transform a vertex, you would do something like...
///    Point2F transformedVertex = transform * vertex;
class Matrix32
{
public:
   typedef float T;

   /// The columns of the matrix.
   Point2<T> m[3];

   /// Return a matrix with the given elements in column-major order.
   static Matrix32 Create(T m11, T m12,
      T m21, T m22,
      T m31, T m32)
   {
      Matrix32 m;

      m[0][0] = m11; m[0][1] = m12;
      m[1][0] = m21; m[1][1] = m22;
      m[2][0] = m31; m[2][1] = m32;

      return m;
   }

   /// Return a matrix with the given columns.
   static Matrix32 Create(const Point2<T> &x, const Point2<T> &y, const Point2<T> &t) { Matrix32 m; m[0] = x; m[1] = y; m[2] = t; return m; }

   /// Make this matrix an identity matrix.
   void IdentitySet()
   {
      m[0].Set(1, 0);
      m[1].Set(0, 1);
      m[2].Set(0, 0);
   }

   /// Without changing the rotation or scale, set the translation of this matrix.
   void TranslationSet(const Point2<T> &p) { m[2] = p; }
   /// Without changing the rotation or scale, set the translation of this matrix.
   void TranslationSet(T x, T y) { m[2].Set(x, y); }

   /// Return a reference to column 'n'.
   Point2<T>& operator[](int n) { return m[n]; }
   /// Return a reference to column 'n'.
   const Point2<T>& operator[](int n) const { return m[n]; }

   /// Return the given column as a Point3<T>.
   Point3<T> GetColumn3(int n) const { return Point3<T>::Create(m[0][n], m[1][n], m[2][n]); }

   // Operators
   Matrix32 operator+() const { return *this; }
   Matrix32 operator-() const { return Create(-m[0], -m[1], -m[2]); }

   /// Return the result of adding all corresponding elements of the two matrices.
   Matrix32 operator+(const Matrix32 &b) const { return Create(m[0]+b[0], m[1]+b[1], m[2]+b[2]); }
   /// Return the result of subtracting all corresponding elements of the second matrix from those of the first.
   Matrix32 operator-(const Matrix32 &b) const { return Create(m[0]-b[0], m[1]-b[1], m[2]-b[2]); }

   /// Return true if all corresponding elements of the two matrices are equal.
   bool operator==(const Matrix32 &b) const { return m[0] == b.m[0] && m[1] == b.m[1] && m[2] == b.m[2]; }

   /// Multiply the two matrices.
   Matrix32 operator*(const Matrix32 &b) const;
   /// Multiply all elements of the matrix by the given value.
   Matrix32 operator*(T k) const { return Create(m[0] * k, m[1] * k, m[2] * k); }
   /// Divide all elements of the matrix by the given value.
   Matrix32 operator/(T k) const { return Create(m[0] / k, m[1] / k, m[2] / k); }

   /// Add all corresponding elements of the given matrix to those of this one.
   Matrix32& operator+=(const Matrix32 &b) { m[0] += b[0]; m[1] += b[1]; m[2] += b[2]; return *this; }
   /// Subtract all corresponding elements of the given matrix from those of this one.
   Matrix32& operator-=(const Matrix32 &b) { m[0] -= b[0]; m[1] -= b[1]; m[2] -= b[2]; return *this; }
   /// Set this matrix to the result of multiplying it by the given matrix.
   Matrix32& operator*=(const Matrix32 &b) { (*this) = (*this) * b; return *this; }
   /// Multiply all elements of the matrix by the given value.
   Matrix32& operator*=(T k) { m[0] *= k; m[1] *= k; m[2] *= k; return *this; }
   /// Divide all elements of the matrix by the given value.
   Matrix32& operator/=(T k) { m[0] /= k; m[1] /= k; m[2] /= k; return *this; }

   // Translation operators

   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   Matrix32& operator+=(const Point2<T>& p) { m[2] += p; return *this; }
   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   Matrix32& operator-=(const Point2<T>& p) { m[2] -= p; return *this; }

   // Transformations

   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   void Translate(const Point2<T> &p) { m[2] += p; }
   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   void Translate(T x, T y) { m[2].x += x; m[2].y += y; }
   /// Apply a rotation, given in degrees, to this matrix.  Assuming that +x is
   /// right and +y is down, the angle increases counter-clockwise.  Equivalent
   /// to (rotation * this)
   void Rotate(T angle);
   /// Apply a scaling to this matrix.  Equivalent to (scale * this)
   void Scale(T k) { (*this) *= k; }
   /// Apply a scaling to this matrix.  Equivalent to (scale * this)
   void Scale(const Point2<T> &s) { m[0] *= s; m[1] *= s; m[2] *= s; }
   /// Apply a scaling to this matrix.  Equivalent to (scale * this)
   void Scale(T x, T y)
   {  m[0][0] *= x; m[0][1] *= y;
      m[1][0] *= x; m[1][1] *= y;
      m[2][0] *= x; m[2][1] *= y;
   }
   /// Equivalent to (this * translation)
   void PreTranslate(const Point2<T> &p)
   {
      m[2][0] = (p.x*m[0][0] + p.y*m[1][0]) + m[2][0];
      m[2][1] = (p.x*m[0][1] + p.y*m[1][1]) + m[2][1];
   }
   /// Equivalent to (this * translation)
   void PreTranslate(T x, T y)
   {
      m[2][0] = (x*m[0][0] + y*m[1][0]) + m[2][0];
      m[2][1] = (x*m[0][1] + y*m[1][1]) + m[2][1];
   }
   /// Equivalent to (this * rotation)
   void PreRotate(T angle);
   /// Equivalent to (this * scale)
   void PreScale(const Point2<T> &s) { m[0] *= s.x; m[1] *= s.y; }
   /// Equivalent to (this * scale)
   void PreScale(T x, T y) { m[0] *= x; m[1] *= y; }

   /// Return the determinant of the 2x2 rotation and scaling part of the matrix.
   friend T Det(const Matrix32& m) { return m[0][0]*m[1][1] - m[0][1]*m[1][0];  }

   /// Return true if this is a right-handed matrix.
   bool IsRightHanded() const { return Det(*this) > 0.0f; }
   /// Return true if this is a left-handed matrix.
   bool IsLeftHanded() const { return Det(*this) < 0.0f; }

   /// Multiply the matrix by the given scalar.
   friend Matrix32 operator*(T k, const Matrix32 &m)
   {
      return Create(k * m[0], k * m[1], m[2] * k);
   }

   /// Multiply this matrix by the given column vector.
   Point2<T> operator*(const Point2<T> &p) const;
   /// Multiply this matrix by the given column vector.
   Point2<T> operator*(const Point3<T> &p) const;

   /// Transform the given column vector without applying translation.  If translation
   /// should be applied, use regular vector multiplication instead of this function.
   Point2<T> TransformVector(const Point2<T>& p) const;

   /// Return the inverse of this matrix.
   friend Matrix32 Inverse(const Matrix32& m);
   /// Invert the given matrix, assuming it is orthonormal with optional translation.
   /// It can have clean rotation, and it can have translation, but it can't have scaling.
   friend Matrix32 InverseOrthonormal(const Matrix32& m);

   static const Matrix32 identity;
};

//==============================================================================

} // namespace Webfoot

#endif //#ifdef __FROG__MATRIX32_H__