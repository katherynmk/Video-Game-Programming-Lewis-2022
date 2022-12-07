#ifndef __FROG__POINT3_H__
#define __FROG__POINT3_H__

#include "FrogMemory.h"
#include "TypeData.h"

//-----------------------------------------------------------------------------

namespace Webfoot
{
//-----------------------------------------------------------------------------

/// \addtogroup DataTypes
/// @{

/// Point3 is a template base struct for coordinates in 3D space.
template<typename T> struct Point3
{
   T  x; ///< X-coordinate
   T  y; ///< Y-coordinate
   T  z; ///< Z-coordinate

   /// Method to create a Point3 from a Point3 of a different type.
   template<typename S>
      static Point3 Create(const Point3<S>& a)  { Point3 p = { (T)a.x, (T)a.y, (T)a.z }; return p;}
   
   /// Method to create a Point3 from provided x,y values.
   static Point3 Create(T x, T y, T z)          { Point3 p = { x, y, z }; return p;    }

   /// Sets the values for this point 
   void Set(const T& aX, const T& aY, const T& aZ) { x = aX; y = aY; z = aZ;           }
   void Set(const T i[3])                    { x = i[0]; y = i[1]; z = i[2];        }

   /// \return Returns the area bounded by (0, 0) and (x, y)
   typename TypeData<T>::MathematicalUpcast VolumeGet() const { return x*y*z;          }

   // Unary operators
   Point3 operator+()               const    { return *this;                     }
   Point3 operator-()               const    { return Create(-x, -y, -z);        }

   // Binary operators
   Point3 operator+(const Point3 &p)   const    { return Create(x+p.x, y+p.y, z+p.z);  }
   Point3 operator-(const Point3 &p)   const    { return Create(x-p.x, y-p.y, z-p.z);  }
   Point3 operator*(const Point3 &p)   const    { return Create(x*p.x, y*p.y, z*p.z);  }
   Point3 operator*(T a)            const    { return Create(x*a,   y*a  , z*a  );  }
   Point3 operator/(const Point3 &p)   const    { return Create(x/p.x, y/p.y, z/p.z);  }
   Point3 operator/(T a)            const    { return Create(x/a,   y/a  , z/a  );  }
   Point3 operator<<(int a)         const    { return Create(x<<a,  y<<a , z<<a );  }
   Point3 operator>>(int a)         const    { return Create(x>>a,  y>>a , z>>a );  }
   void operator+=(const Point3 &p)          { x += p.x; y += p.y; z += p.z;        }
   void operator-=(const Point3 &p)          { x -= p.x; y -= p.y; z -= p.z;        }
   void operator*=(const Point3 &p)          { x *= p.x; y *= p.y; z *= p.z;        }
   void operator*=(T a)                   { x *= a;   y *= a;    z *= a;         }
   void operator/=(const Point3 &p)          { x /= p.x; y /= p.y; z /= p.z;        }
   void operator/=(T a)                   { x /= a;   y /= a;    z /= a;         }
   void operator<<=(int a)                   { x <<= a;  y <<= a;  z <<= a;         }
   void operator>>=(int a)                   { x >>= a;  y >>= a;  z >>= a;         }

   /// Dot product
   inline typename TypeData<T>::MathematicalUpcast operator%(const Point3 &p) const { return x*p.x + y*p.y + z*p.z;  }
   
   /// Cross product
   Point3 operator^(const Point3 &p)   const    { return Create(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x);  }
   
   /// Array access method. p[0] is equivalent to p.x and p[1] is equivalent to p.y, p[2] is equivalent to p.z
   /// This allows for algorithms that use loop indices into the point.
   const T& operator[](int n)       const    { return (&x)[n];                }
        T& operator[](int n)                 { return (&x)[n];                }

   bool operator==(const Point3 &p) const    { return x==p.x && y==p.y && z==p.z;   }
   bool operator!=(const Point3 &p) const    { return x!=p.x || y!=p.y || z!=p.z;   }
   bool operator< (const Point3 &p) const    { return x< p.x && y< p.y && z< p.z;   }
   bool operator> (const Point3 &p) const    { return x> p.x && y> p.y && z> p.z;   }
   bool operator<=(const Point3 &p) const    { return x<=p.x && y<=p.y && z<=p.z;   }
   bool operator>=(const Point3 &p) const    { return x>=p.x && y>=p.y && z>=p.z;   }

   friend Point3 abs(const Point3 &p)           { return Create(abs(p.x), abs(p.y), abs(p.z)); }

   friend typename TypeData<T>::MathematicalUpcast LengthSquared(const Point3 &p)   { return p.x*p.x + p.y*p.y + p.z*p.z;  }

   friend Point3 operator*(T a, const Point3 &p)   { return Create(a*p.x, a*p.y, a*p.z);  }

   // TODO: figure out why gcc is complaining about these prototypes when we implement them
   // Implemented for Point3<float> in FrogMath.cpp   
   friend Point3<float> Normalize(const Point3<float>& p);

   // Implemented for Point3<float> in FrogMath.cpp   
   friend float Length(const Point3<float>& p);

   /// Implemented for Point3<float> in FrogMath.cpp   
   friend Point3<float> LengthClamp(const Point3<float>& p, float minLength, float maxLength);
};

float Length(const Point3<float>& p);

/// Return a copy of the vector with its length clamped to the given range.
Point3<float> LengthClamp(const Point3<float>& p, float minLength, float maxLength);

typedef Point3<int> Point3I;
typedef Point3<float> Point3F;

/// @}
//-----------------------------------------------------------------------------
} // namespace Webfoot
//-----------------------------------------------------------------------------

#endif //#ifndef __FROG__POINT3_H__
