#ifndef __FROG__MATRIX43_H__
#define __FROG__MATRIX43_H__

#include "FrogMemory.h"
#include "Point3.h"
#include "Point4.h"
#include "Quaternion.h"
#include "Matrix32.h"

namespace Webfoot {

//==============================================================================

const unsigned MatrixFlag_NoScale = 1;
const unsigned MatrixFlag_NoRotation = 2;
const unsigned MatrixFlag_NoTranslation = 4;
const unsigned MatrixFlag_Identity = 7;

/// Matrix43 is a 4x4 column-major matrix where the bottom row is locked to [0, 0, 0, 1].
/// When working with vertices, they will be treated as column-vectors.
/// When building transforms, apply them from right to left.  For example, if you had
/// separate matrices for scaling, rotation, and translation, and you wanted to
/// apply them in that order, the code would look like...
///    Matrix43 transform = translation * rotation * scaling;
/// To then transform a vertex, you would do something like...
///    Point3F transformedVertex = transform * vertex;
///
/// Matrix43 has an optional set of flags for when a matrix has certain
/// properties that can make multiplication less expensive.  For example,
/// if you build a matrix that is just for translation, you could do...
///    translationMatrix.flags = MatrixFlag_NoScale | MatrixFlag_NoRotation;
/// Be careful when changing the elements directly, changing the rows, or changing
/// the columns with SetColumn, because that won't automatically remove any
/// flags that are no longer appropriate.
class Matrix43
{
public:
   typedef float T;

   /// The columns of the matrix.  Beware that changing matrix
   /// entries directly will not automatically update the optimization flags.
   Point3<T> m[4];
   /// Properties of the matrix that can make multiplication less expensive.
   unsigned	flags;

   Matrix43() { flags = 0; }

#ifndef _DEBUG
   void AssertFlags() { }
#else
   void AssertFlags();
#endif

   /// Return a matrix with the given elements in column-major order.
   static Matrix43 Create(T m11, T m12, T m13,
                          T m21, T m22, T m23,
                          T m31, T m32, T m33,
                          T m41, T m42, T m43,
                          unsigned flags = 0)
   {
      Matrix43 m;

      m[0][0] = m11; m[0][1] = m12; m[0][2] = m13;
      m[1][0] = m21; m[1][1] = m22; m[1][2] = m23;
      m[2][0] = m31; m[2][1] = m32; m[2][2] = m33;
      m[3][0] = m41; m[3][1] = m42; m[3][2] = m43;

      m.flags = flags;
      return m;
   }

   /// Return a matrix with the given columns.
   static Matrix43 Create(const Point3<T> &x, const Point3<T> &y,
                          const Point3<T> &z, const Point3<T> &t, unsigned flags=0)
   {
      Matrix43 m;

      m[0] = x;
      m[1] = y;
      m[2] = z;
      m[3] = t;
   
      m.flags = flags;
      return m;
   }

   /// Convert the given Matrix32 to a Matrix43.
   static Matrix43 Create(const Matrix32& other)
   {
      Matrix43 m;

      m.m[0].x = other.m[0].x; m.m[1].x = other.m[1].x; m.m[2].x = 0.0f; m.m[3].x = other.m[2].x;
      m.m[0].y = other.m[0].y; m.m[1].y = other.m[1].y; m.m[2].y = 0.0f; m.m[3].y = other.m[2].y;
      m.m[0].z = 0.0f; m.m[1].z = 0.0f; m.m[2].z = 1.0f; m.m[3].z = 0.0f;

      return m;
   }

   /// Return a matrix representation of the given rotation quaternion.
   /// This will not set any flags.
   static Matrix43 Create(const Quaternion& q) { Matrix43 m; m.RotationSet(q); return m; }

   /// Set this matrix to represent the given rotation quaternion.
   /// This will not set any flags.
   void RotationSet(const Quaternion& q);
   /// Assuming this is an orthonormal rotation matrix (no scaling), return
   /// the Euler angles for this rotation assuming they are applied in the
   /// order X, Y, Z.  This is susceptible to gimbal lock when the Y angle is
   /// near +/- 90 degrees.
   Point3F RotationEulerXYZGet() const;

   /// Make this matrix an identity matrix.
   /// This will not set any flags.
   void IdentitySet()
   {
      m[0].Set(1, 0, 0);
      m[1].Set(0, 1, 0);
      m[2].Set(0, 0, 1);
      m[3].Set(0, 0, 0);
   }

   /// Without changing the rotation or scale, set the translation of this matrix.
   void TranslationSet(const Point3<T> &p) { m[3] = p; flags &= ~MatrixFlag_NoTranslation; }
   /// Without changing the rotation or scale, set the translation of this matrix.
   void TranslationSet(T x, T y, T z) { m[3].Set(x, y, z); flags &= ~MatrixFlag_NoTranslation; }

   /// Return a reference to column 'n'.  Beware that changing matrix
   /// entries through this will not automatically update the optimization flags.
   Point3<T>& operator[](int n) { return m[n]; }
   /// Return column 'n'.
   const Point3<T>& operator[](int n) const { return m[n]; }

   /// Return the given row of the matrix.
   Point3<T> Row3Get(int n) const { return Point3<T>::Create(m[0][n], m[1][n], m[2][n]); }
   /// Set the given row of the matrix.  Beware that changing matrix
   /// entries through this will not automatically update the optimization flags.
   /// The fourth column will remain unchanged.
   void RowSet(int n, const Point3<T>& row) { m[0][n] = row.x; m[1][n] = row.y; m[2][n] = row.z; }

   // Operators
   Matrix43 operator+() const	{ return *this; }
   /// Return the result of multiplying all elements of this matrix by -1.
   Matrix43 operator-() const	{ return Create(-m[0], -m[1], -m[2], -m[3], flags & ~MatrixFlag_NoScale); }

   /// Return the result of adding all corresponding elements of the two matrices.
   Matrix43 operator+(const Matrix43 &b) const { return Create(m[0]+b[0], m[1]+b[1], m[2]+b[2], m[3]+b[3]);	}
   /// Return the result of subtracting all corresponding elements of the second matrix from those of the first.
   Matrix43 operator-(const Matrix43 &b) const { return Create(m[0]-b[0], m[1]-b[1], m[2]-b[2], m[3]-b[3]);	}

   /// Return true if all corresponding elements of the two matrices are equal.
   bool operator==(const Matrix43 &b) const { return m[0] == b.m[0] && m[1] == b.m[1] && m[2] == b.m[2] && m[3] == b.m[3]; }

   /// Multiply the two matrices.
   Matrix43 operator*(const Matrix43 &b) const;
   /// Multiply all elements of the matrix by the given value.
   Matrix43 operator*(T k) const { return Create(m[0] * k, m[1] * k, m[2] * k, m[3] * k, flags & ~MatrixFlag_NoScale); }
   /// Divide all elements of the matrix by the given value.
   Matrix43 operator/(T k) const { return Create(m[0] / k, m[1] / k, m[2] / k, m[3] / k, flags & ~MatrixFlag_NoScale); }

   /// Add all corresponding elements of the given matrix to those of this one.
   Matrix43& operator+=(const Matrix43 &b) { m[0] += b[0]; m[1] += b[1]; m[2] += b[2]; m[3] += b[3]; flags = 0; return *this; }
   /// Subtract all corresponding elements of the given matrix from those of this one.
   Matrix43& operator-=(const Matrix43 &b) { m[0] -= b[0]; m[1] -= b[1]; m[2] -= b[2]; m[3] -= b[3]; flags = 0; return *this; }
   /// Set this matrix to the result of multiplying it by the given matrix.
   Matrix43& operator*=(const Matrix43 &b) { (*this) = (*this) * b; return *this; }
   /// Multiply all elements of the matrix by the given value.
   Matrix43& operator*=(const T& k) { m[0] *= k; m[1] *= k; m[2] *= k; m[3] *= k; flags &= ~MatrixFlag_NoScale; return *this; }
   /// Divide all elements of the matrix by the given value.
   Matrix43& operator/=(const T& k) { m[0] /= k; m[1] /= k; m[2] /= k; m[3] /= k; flags &= ~MatrixFlag_NoScale; return *this; }

   // Translation operators

   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   Matrix43& operator+=(const Point3<T>& p) { m[3] += p; flags &= ~MatrixFlag_NoTranslation; return *this; }
   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   Matrix43& operator-=(const Point3<T>& p) { m[3] -= p; flags &= ~MatrixFlag_NoTranslation; return *this; }

   // Transformations

   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   void Translate(const Point3<T> &p) { m[3] += p; flags &= ~MatrixFlag_NoTranslation; }
   /// Apply a translation to this matrix.  Equivalent to (translation * this)
   void Translate(T x, T y, T z) { m[3].x += x; m[3].y += y; m[3].z += z; flags &= ~MatrixFlag_NoTranslation; }
   /// Apply a rotation to this matrix.  Equivalent to (rotation * this)
   void Rotate(Quaternion rotation) { Matrix43 rotationMatrix; rotationMatrix.RotationSet(rotation); *this = rotationMatrix * *this; }
   /// Apply a scaling to this matrix.  Equivalent to (scale * this)
   void Scale(T k) { (*this) *= k; flags &= ~MatrixFlag_NoScale; }
   /// Apply a scaling to this matrix.  Equivalent to (scale * this)
   void Scale(const Point3<T> &s) { m[0] *= s; m[1] *= s; m[2] *= s; m[3] *= s; flags &= ~MatrixFlag_NoScale; }
   /// Apply a scaling to this matrix.  Equivalent to (scale * this)
   void Scale(T x, T y, T z)
   {
      m[0][0] *= x; m[0][1] *= y; m[0][2] *= z;
      m[1][0] *= x; m[1][1] *= y; m[1][2] *= z;
      m[2][0] *= x; m[2][1] *= y; m[2][2] *= z;
      m[3][0] *= x; m[3][1] *= y; m[3][2] *= z;
      flags &= ~MatrixFlag_NoScale;
   }
   /// Equivalent to (this * translation)
   void PreTranslate(const Point3<T> &p)
   {
      m[3][0] = static_cast<T>(p.x*m[0][0] + p.y*m[1][0] + p.z*m[2][0]) + m[3][0];
      m[3][1] = static_cast<T>(p.x*m[0][1] + p.y*m[1][1] + p.z*m[2][1]) + m[3][1];
      m[3][2] = static_cast<T>(p.x*m[0][2] + p.y*m[1][2] + p.z*m[2][2]) + m[3][2];
      flags &= ~MatrixFlag_NoTranslation;
   }
   /// Equivalent to (this * translation)
   void PreTranslate(T x, T y, T z)
   { 
      m[3][0] = static_cast<T>(x*m[0][0] + y*m[1][0] + z*m[2][0]) + m[3][0];
      m[3][1] = static_cast<T>(x*m[0][1] + y*m[1][1] + z*m[2][1]) + m[3][1];
      m[3][2] = static_cast<T>(x*m[0][2] + y*m[1][2] + z*m[2][2]) + m[3][2];
      flags &= ~MatrixFlag_NoTranslation;
   }
   /// Equivalent to (this * rotation)
   void PreRotate(Quaternion rotation) { Matrix43 rotationMatrix; rotationMatrix.RotationSet(rotation); *this = *this * rotationMatrix; }
   /// Equivalent to (this * scale)
   void PreScale(const Point3<T> &s) { m[0] *= s.x; m[1] *= s.y; m[2] *= s.z; flags &= ~MatrixFlag_NoScale; }
   /// Equivalent to (this * scale)
   void PreScale(T x, T y, T z) { m[0] *= x; m[1] *= y; m[2] *= z; flags &= ~MatrixFlag_NoScale; }

   /// Return the determinant of the 3x3 rotation and scaling part of the matrix.
   friend T Det(const Matrix43& m)
   {
      return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
           - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
           + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
   }

   /// Return true if this is a right-handed matrix.
   bool RightHandedCheck()	const { return Det(*this) > 0; }
   /// Return true if this is a left-handed matrix.
   bool LeftHandedCheck()	const { return Det(*this) < 0; }

   /// Multiply the matrix by the given scalar.
   friend Matrix43 operator*(T k, const Matrix43 &m)
   {
      return Create(k * m[0], k * m[1], m[2] * k, m[3] * k, m.flags & ~MatrixFlag_NoScale);
   }

   /// Multiply this matrix by the given column vector.
   Point3<T> operator*(const Point3<T> &p) const;

   /// Multiply this matrix by the given column vector.
   Point3<T> operator*(const Point4<T> &p) const
   {
      return Point3<T>::Create(p.x*m[0][0] + p.y*m[1][0] + p.z*m[2][0] + p.w*m[3][0],
                               p.x*m[0][1] + p.y*m[1][1] + p.z*m[2][1] + p.w*m[3][1],
                               p.x*m[0][2] + p.y*m[1][2] + p.z*m[2][2] + p.w*m[3][2]);
   }

   /// Transform the given column vector without applying translation.  If translation
   /// should be applied, use regular vector multiplication instead of this function.
   Point3<T> VectorTransform(const Point3<T>& p) const;

   /// Invert the given matrix, assuming it is orthonormal with optional translation.
   /// It can have clean rotation, and it can have translation, but it can't have scaling.
   friend Matrix43 Inverse(const Matrix43& m);

   /// An identity matrix
   static const Matrix43 identity;
};

/// Invert the given matrix, assuming it is orthonormal with optional translation.
/// It can have clean rotation, and it can have translation, but it can't have scaling.
Matrix43 Inverse(const Matrix43& m);

//==============================================================================

} // namespace Webfoot

#endif //#ifdef __FROG__MATRIX43_H__
