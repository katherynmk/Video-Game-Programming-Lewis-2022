#ifndef __FROG__POINT4_H__
#define __FROG__POINT4_H__

#include "FrogMemory.h"
#include "TypeData.h"

//-----------------------------------------------------------------------------

namespace Webfoot
{
//-----------------------------------------------------------------------------

/// \addtogroup DataTypes
/// @{

/// Point4 is a template base struct for coordinates in 4D space.
template<typename T> struct Point4
{
   T  x; ///< X-coordinate
   T  y; ///< Y-coordinate
   T  z; ///< Z-coordinate
   T  w; ///< W-coordinate

   /// Method to create a Point4 from a Point4 of a different type.
   template<typename S>
      static Point4 Create(const Point4<S>& a)  { Point4 p = { (T)a.x, (T)a.y, (T)a.z, (T)a.w }; return p; }
   
   /// Method to create a Point4 from provided x,y values.
   static Point4 Create(T x, T y, T z, T w)     { Point4 p = { x, y, z, w }; return p;    }

   /// Sets the values for this point 
   void Set(const T& aX, const T& aY, const T& aZ, const T& aW)   { x = aX; y = aY; z = aZ; w = aW;         }
   void Set(const T i[4])                                { x = i[0]; y = i[1]; z = i[2]; w = i[3]; }

   // Unary operators
   Point4 operator+()               const    { return *this;                     }
   Point4 operator-()               const    { return Create(-x, -y, -z, -w);    }

   // Binary operators
   Point4 operator+(const Point4 &p)   const    { return Create(x+p.x, y+p.y, z+p.z, w+p.w); }
   Point4 operator-(const Point4 &p)   const    { return Create(x-p.x, y-p.y, z-p.z, w-p.w); }
   Point4 operator*(const Point4 &p)   const    { return Create(x*p.x, y*p.y, z*p.z, w*p.w); }
   Point4 operator*(T a)            const    { return Create(x*a,   y*a  , z*a  , w*a  ); }
   Point4 operator/(const Point4 &p)   const    { return Create(x/p.x, y/p.y, z/p.z, w/p.w); }
   Point4 operator/(T a)            const    { return Create(x/a,   y/a  , z/a  , w/a  ); }
   Point4 operator<<(int a)         const    { return Create(x<<a,  y<<a , z<<a , w<<a ); }
   Point4 operator>>(int a)         const    { return Create(x>>a,  y>>a , z>>a , w>>a ); }
   void operator+=(const Point4 &p)          { x += p.x; y += p.y; z += p.z; w += p.w;    }
   void operator-=(const Point4 &p)          { x -= p.x; y -= p.y; z -= p.z; w -= p.w;    }
   void operator*=(const Point4 &p)          { x *= p.x; y *= p.y; z *= p.z; w *= p.w;    }
   void operator*=(T a)                   { x *= a;   y *= a;    z *= a;   w *= a;        }
   void operator/=(const Point4 &p)          { x /= p.x; y /= p.y; z /= p.z;  w /= p.w;      }
   void operator/=(T a)                   { x /= a;   y /= a;    z /= a;   w /= a;        }
   //void operator<<=(int a)                   { x <<= a;  y <<= a;  z <<= a;   w <<= a;    }
   //void operator>>=(int a)                   { x >>= a;  y >>= a;  z >>= a;   w >>= a;    }

   /// Dot product
   typename TypeData<T>::MathematicalUpcast operator%(const Point4 &p) const { return x*p.x + y*p.y + z*p.z + w*p.w; }
   
   /// Array access method. p[0] is equivalent to p.x and p[1] is equivalent to p.y, p[2] is equivalent to p.z
   /// This allows for algorithms that use loop indices into the point.
   const T& operator[](int n)       const    { return (&x)[n];                      }
        T& operator[](int n)                 { return (&x)[n];                      }

   bool operator==(const Point4 &p) const    { return x==p.x && y==p.y && z==p.z && w==p.w;  }
   bool operator!=(const Point4 &p) const    { return x!=p.x || y!=p.y || z!=p.z || w!=p.w;  }
   bool operator< (const Point4 &p) const    { return x< p.x && y< p.y && z< p.z && w< p.w;  }
   bool operator> (const Point4 &p) const    { return x> p.x && y> p.y && z> p.z && w> p.w;  }
   bool operator<=(const Point4 &p) const    { return x<=p.x && y<=p.y && z<=p.z && w<=p.w;  }
   bool operator>=(const Point4 &p) const    { return x>=p.x && y>=p.y && z>=p.z && w>=p.w;  }

   friend Point4 abs(const Point4 &p)           { return Create(abs(p.x), abs(p.y), abs(p.z), abs(p.w)); }

   friend typename TypeData<T>::MathematicalUpcast LengthSquared(const Point4 &p)   { return p.x*p.x + p.y*p.y + p.z*p.z + p.w*p.w; }

   friend Point4 operator*(T a, const Point4 &p)   { return Create(a*p.x, a*p.y, a*p.z, a*p.w); }
};

typedef Point4<int> Point4I;
typedef Point4<float> Point4F;

/// @}
//-----------------------------------------------------------------------------
} // namespace Webfoot
//-----------------------------------------------------------------------------

#endif //#ifndef __FROG__POINT4_H__
