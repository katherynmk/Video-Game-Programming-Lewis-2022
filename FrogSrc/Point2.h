#ifndef __FROG__POINT2_H__
#define __FROG__POINT2_H__

#include "FrogMemory.h"
#include "TypeData.h"

//-----------------------------------------------------------------------------

namespace Webfoot
{
//-----------------------------------------------------------------------------

/// \addtogroup DataTypes
/// @{

/// Point2 is a template base struct for coordinates in 2D space.
template<typename T> struct Point2
{
   T  x; ///< X-coordinate
   T  y; ///< Y-coordinate

   /// Method to create a Point2 from a Point2 of a different type.
   template<typename S>
      static Point2 Create(const Point2<S>& a)  { Point2 p = { (T)a.x, (T)a.y }; return p;}
   
   /// Method to create a Point2 from provided x,y values.
   static Point2 Create(T x, T y)               { Point2 p = { x, y }; return p;    }

   /// Sets the values for this point 
   void Set(const T& aX, const T& aY)           { x = aX; y = aY;             }
   void Set(const T i[2])                    { x = i[0]; y = i[1];            }

   /// \return Returns the area bounded by (0, 0) and (x, y)
   typename TypeData<T>::MathematicalUpcast AreaGet() const { return (typename TypeData<T>::MathematicalUpcast)(x*y);            }

   /** This is a convenience function for accessing conceptual 2D arrays that are linearised.
    For example:
   \code 
   unsigned char buffer[displayWidth*displayHeight]; 

   buffer[p.IndexForWidthGet(displayWidth)] = 0;
   \endcode
   **/
   int      IndexForWidthGet(int arrayWidth) const { return y*arrayWidth + x;       }

   // Unary operators
   Point2 operator+()               const    { return *this;                  }
   Point2 operator-()               const    { return Create(-x, -y);         }

   // Binary operators
   Point2 operator+(const Point2 &p)   const    { return Create(x+p.x, y+p.y);      }
   Point2 operator-(const Point2 &p)   const    { return Create(x-p.x, y-p.y);      }
   Point2 operator*(const Point2 &p)   const    { return Create(x*p.x, y*p.y);      }
   Point2 operator*(T a)            const    { return Create(x*a,   y*a  );      }
   Point2 operator/(const Point2 &p)   const    { return Create(x/p.x, y/p.y);      }
   Point2 operator/(T a)            const    { return Create(x/a,   y/a  );      }
   Point2 operator<<(int a)         const    { return Create(x<<a,  y<<a );      }
   Point2 operator>>(int a)         const    { return Create(x>>a,  y>>a );      }
   void operator+=(const Point2 &p)          { x += p.x; y += p.y;            }
   void operator-=(const Point2 &p)          { x -= p.x; y -= p.y;            }
   void operator*=(const Point2 &p)          { x *= p.x; y *= p.y;            }
   void operator*=(T a)                   { x *= a;   y *= a;              }
   void operator/=(const Point2 &p)          { x /= p.x; y /= p.y;            }
   void operator/=(T a)                   { x /= a;   y /= a;              }
   void operator<<=(int a)                   { x <<= a;  y <<= a;          }
   void operator>>=(int a)                   { x >>= a;  y >>= a;          }

   /// Dot product
   typename TypeData<T>::MathematicalUpcast operator%(const Point2 &p) const { return x*p.x + y*p.y;  }
   
   /// Array access method. p[0] is equivalent to p.x and p[1] is equivalent to p.y
   /// This allows for algorithms that use loop indices into the point.
   const T& operator[](int n)       const    { return (&x)[n];             }
        T& operator[](int n)                 { return (&x)[n];             }

   bool operator==(const Point2 &p) const    { return x==p.x && y==p.y;       }
   bool operator!=(const Point2 &p) const    { return x!=p.x || y!=p.y;       }
   bool operator< (const Point2 &p) const    { return x< p.x && y< p.y;       }
   bool operator> (const Point2 &p) const    { return x> p.x && y> p.y;       }
   bool operator<=(const Point2 &p) const    { return x<=p.x && y<=p.y;       }
   bool operator>=(const Point2 &p) const    { return x>=p.x && y>=p.y;       }

   friend Point2 abs(const Point2 &p)           { return Create(abs(p.x), abs(p.y)); }

   friend typename TypeData<T>::MathematicalUpcast LengthSquared(const Point2 &p)   { return p.x*p.x + p.y*p.y;   }

   friend Point2 operator*(T a, const Point2 &p)   { return Create(a*p.x, a*p.y);      }
   
   Point2<int> AsInteger( ) const               { return Point2<int>::Create( x.AsInteger( ), y.AsInteger( )); }

   // TODO: figure out why gcc is complaining about these prototypes when we implement them
   // Implemented for Point2<float> in FrogMath.cpp   
   friend Point2<float> Normalize(const Point2<float>& p);

   // Implemented for Point2<float> in FrogMath.cpp   
   friend float Length(const Point2<float>& p);

   /// Implemented for Point2<float> in FrogMath.cpp   
   friend Point2<float> LengthClamp(const Point2<float>& p, float minLength, float maxLength);
};

float Length(const Point2<float>& p);

/// Return a copy of the vector with its length clamped to the given range.
Point2<float> LengthClamp(const Point2<float>& p, float minLength, float maxLength);

typedef Point2<int> Point2I;
typedef Point2<float> Point2F;

/// @}
//-----------------------------------------------------------------------------
} // namespace Webfoot
//-----------------------------------------------------------------------------

#endif //#ifndef __FROG__POINT2_H__
