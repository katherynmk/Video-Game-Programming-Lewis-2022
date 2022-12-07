#ifndef __FROG__BOX3_H__
#define __FROG__BOX3_H__

#include "FrogMemory.h"
#include <algorithm>
#include "Point3.h"
#include "TypeData.h"

//-----------------------------------------------------------------------------
namespace Webfoot
{

/// \addtogroup DataTypes
/// @{

/// Box3 is a template struct to represent a region in 3D space.
/// Operations involving width, height, and depth are exclusive rather than inclusive.
template<typename T> class Box3
{
public:
   /// position (inclusive) coordinate for the left (minimum) side of the region.
   T x;
   /// position (inclusive) coordinate for the top (minimum) side of the region.
   T y;
   /// position (inclusive) coordinate for the near (minimum) side of the region.
   T z;
   /// width of the region.  The difference between the maximum x and x.
   T width;
   /// height of the region.  The difference between the maximum y and x.
   T height;
   /// depth of the region.  The difference between the maximum z and z.
   T depth;

   /// List of the corners of the box for use with CornerGet.
   enum Corner
   {
      TOP_LEFT_NEAR,
      TOP_RIGHT_NEAR,
      BOTTOM_LEFT_NEAR,
      BOTTOM_RIGHT_NEAR,
      TOP_LEFT_FAR,
      TOP_RIGHT_FAR,
      BOTTOM_LEFT_FAR,
      BOTTOM_RIGHT_FAR
   };

   /// Method to create a Box3 from a Box3 of a different type.
   template<typename S>
   static Box3 Create(const Box3<S>& a)
   {
      Box3 b = {(T)a.x, (T)a.y, (T)a.z, (T)a.width, (T)a.height, (T)a.depth};
      return b;
   }

   static Box3 Create(T _x, T _y, T _z, T _width, T _height, T _depth)
   {
      Box3 b = { _x, _y, _z, _width, _height, _depth };
      return b;
   }
   static Box3 Create(const Point3<T>& aMin, const Point3<T>& aMax)
   {
      Box3 b = { aMin.x, aMin.y, aMin.z, aMax.x-aMin.x, aMax.y-aMin.y, aMax.z-aMin.z };
      return b;
   }

   void Set(T _x, T _y, T _z, T _width, T _height, T _depth)
   {
      x=_x; y=_y; z=_z; width=_width; height=_height; depth=_depth;
   }
   void Set(const Point3<T> &aMin, const Point3<T> &aMax)
   {
      x=aMin.x; y=aMin.y; z=aMin.z; width=aMax.x-aMin.x; height=aMax.y-aMin.y; depth=aMax.z-aMin.z;
   }

   typename TypeData<T>::MathematicalUpcast WidthGet()  const  { return width; }
   typename TypeData<T>::MathematicalUpcast HeightGet() const  { return height; }
   typename TypeData<T>::MathematicalUpcast DepthGet()  const  { return depth; }

   Point3<T> SizeGet() const                    { return Point3<T>::Create(width, height, depth); }
   typename TypeData<T>::MathematicalUpcast VolumeGet() const  { return WidthGet() * HeightGet() * DepthGet(); }

   bool     EmptyCheck()      const                { return ((width == 0) || (height == 0) || (depth == 0)); }
   void     EmptySet()                          { x = y = z = width = height = depth = 0; }

   void     WidthSet(T _width)                      { width = _width; }
   void     HeightSet(T _height)                    { height = _height; }
   void     DepthSet(T _depth)                      { depth = _depth; }
   void     SizeSet(T _width, T _height, T _depth)  { WidthSet(_width); HeightSet(_height); DepthSet(_depth); }
   void     SizeSet(const Point3<T> &size)          { width = size.x; height = size.y; depth = size.z; }

   /// Set x, y, and z without affecting width, height, and depth.
   void PositionSet(const Point3<T>& newPosition) { x = newPosition.x; y = newPosition.y; z = newPosition.z; }

   /// Return the near corner of the box
   Point3<T> MinGet() const                     { return Point3<T>::Create(x, y, z); }
   /// Set the near x coordinate of the box without changing the far x coordinate.
   /// This may change the width.
   void MinXSet(const T& minX)                  { width += (x - minX); x = minX; }
   /// Set the near y coordinate of the box without changing the far y coordinate.
   /// This may change the height.
   void MinYSet(const T& minY)                  { height += (y - minY); y = minY; }
   /// Set the near z coordinate of the box without changing the far z coordinate.
   /// This may change the depth.
   void MinZSet(const T& minZ)                  { depth += (z - minZ); z = minZ; }
   /// Set the near corner of the box without changing the far corner.
   /// This may change the width, height, and depth.
   void MinSet(const Point3<T>& _min)           { MinXSet(_min.x); MinYSet(_min.y); MinZSet(_min.z); }

   /// Return the far x coordinate of the box.
   typename TypeData<T>::MathematicalUpcast MaxXGet() const { return x+width; }
   /// Return the far y coordinate of the box.
   typename TypeData<T>::MathematicalUpcast MaxYGet() const { return y+height; }
   /// Return the far z coordinate of the box.
   typename TypeData<T>::MathematicalUpcast MaxZGet() const { return z+depth; }
   /// Return the far corner of the box.
   Point3<T> MaxGet() const { return Point3<T>::Create(x+width, y+height, z+depth); }

   /// Set the far x coordinate of the box.
   void MaxXSet(const T& maxX)                  { width = maxX-x; }
   /// Set the far y coordinate of the box.
   void MaxYSet(const T& maxY)                  { height = maxY-y; }
   /// Set the far z coordinate of the box.
   void MaxZSet(const T& maxZ)                  { depth = maxZ-z; }
   /// Set the far corner of the box.
   void MaxSet(const Point3<T>& max)            { width = max.x-x; height = max.y-y; depth = max.z-z; }

   /// Return true if the width, height, and depth are all non-negative.
   bool ValidCheck() const { return (width >= 0) && (height >= 0) && (depth >= 0); }

   Point3<T> MidPointGet() const { return Point3<T>::Create(x + (width/2), y + (height/2), z + (depth/2)); }

   // Translation operators
   Box3     operator+(const Point3<T> &t) const       { return Create(x+t.x, y+t.y, z+t.z, width, height, depth); }
   Box3     operator-(const Point3<T> &t) const       { return Create(x-t.x, y-t.y, z-t.z, width, height, depth); }
   void     operator+=(const Point3<T> &t)            { x += t.x; y += t.y; z += t.z; }
   void     operator-=(const Point3<T> &t)            { x -= t.x; y -= t.y; z -= t.z; }

   // Scale operators
   Box3     operator*(const Point3<T> &t) const       { return Create(x*t.x, y*t.y, z*t.z, width*t.x, height*t.y, depth*t.z); }
   Box3     operator/(const Point3<T> &t) const       { return Create(x/t.x, y/t.y, z/t.z, width/t.x, height/t.y, depth/t.z); }
   void     operator*=(const Point3<T> &t)            { x *= t.x; y *= t.y; z *= t.z; width *= t.x; height *= t.y; depth *= t.z; }
   void     operator/=(const Point3<T> &t)            { x /= t.x; y /= t.y; z /= t.z; width /= t.x; height /= t.y; depth /= t.z; }
   Box3     operator*(const T &t) const       { return Create(x*t, y*t, z*t, width*t, height*t, depth*t); }
   Box3     operator/(const T &t) const       { return Create(x/t, y/t, z/t, width/t, height/t, depth/t); }
   void     operator*=(const T &t)            { x *= t; y *= t; z *= t; width *= t; height *= t; depth *= t; }
   void     operator/=(const T &t)            { x /= t; y /= t; z /= t; width /= t; height /= t; depth /= t; }

   /// Bit shift the position to the right
   Box3     operator>>(int i) const                   { return Create(x>>i, y>>i, z>>i, width, height, depth); }
   /// Bit shift the position to the left
   Box3     operator<<(int i) const                   { return Create(x<<i, y<<i, z<<i, width, height, depth); }

   // Array access
   const typename TypeData<T>::MathematicalUpcast& operator[](int n) const           { return (&x)[n]; }
        typename TypeData<T>::MathematicalUpcast& operator[](int n)                  { return (&x)[n]; }

   inline bool operator==( const Box3& b ) const
   {
      return( (x == b.x) && (y == b.y) && (z == b.z) && (width == b.width) && (height == b.height) && (depth == b.depth) );
   }
   inline bool operator!=( const Box3& b ) const
   {
      return( (x != b.x) || (y != b.y) || (z != b.z) || (width != b.width) || (height != b.height) || (depth != b.depth) );
   }

   /// Return a point for the given corner.
   Point3<T> CornerGet(Corner corner) const;

   /// Return the box expanded in all directions by r units.
   friend Box3 Expand(const Box3 &b, int r)
   {
      return Create(b.x - r, b.y - r, b.z - r, b.width + (r*2), b.height + (r*2), b.depth + (r*2));
   }

   /// Return true if this box contains the given box.
   inline bool ContainsCheck(const Box3 &b) const          { return x <= b.x && 
                                                               y <= b.y &&
                                                               z <= b.z &&
                                                               x+width >= b.x+b.width &&
                                                               y+height >= b.y+b.height &&
                                                               z+depth >= b.z+b.depth; }

   /// Tests if a point is within the box.
   inline bool ContainsCheck(const Point3<T> &p) const;
   /// Tests if a point is within the box.
   inline bool operator&(const Point3<T> &p) const { return ContainsCheck(p); }
   /// Tests if a point is within the box.
   inline bool operator%(const Point3<T> &p) const { return ContainsCheck(p); }

   /// Tests if two boxes overlap
   inline bool OverlapCheck(const Box3 &b) const;
   /// Tests if two boxes overlap
   inline bool operator%(const Box3 &b) const { return OverlapCheck(b); }

   /// Returns the intersection of two boxes 
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   Box3 Intersection(const Box3 &b) const;
   /// Returns the intersection of two boxes 
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   Box3 operator&(const Box3 &b) const { return Intersection(b); }

   /// Set this box to the intersection of it and the given box.
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   void SetToIntersection(const Box3 &b);
   /// Set this box to the intersection of it and the given box.
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   void operator&=(const Box3 &b) { SetToIntersection(b); }

   /// Returns the union of this box with another - assumes neither is empty
   Box3 Union(const Box3 &b) const;
   /// Returns the union of this box with another - assumes neither is empty
   Box3 operator|(const Box3 &b) const { return Union(b); }

   /// Set this box to the union of it and the given box.
   void SetToUnion(const Box3 &b);
   /// Set this box to the union of it and the given box.
   void operator|=(const Box3 &b) { SetToUnion(b); }

   /// Returns a copy of the box expanded to include the given point.
   Box3 Union(const Point3<T> &p) const;
   /// Returns a copy of the box expanded to include the given point.
   Box3 operator|(const Point3<T> &p) const { return Union(p); }

   /// Expand the box to include the given point
   void SetToUnion(const Point3<T> &p);
   /// Expand the box to include the given point
   void operator|=(const Point3<T> &p) { SetToUnion(p); }
};

//-----------------------------------------------------------------------------

template<typename T> Point3<T> Box3<T>::CornerGet(Corner corner) const
{
   switch(corner)
   {
   default:
   case TOP_LEFT_NEAR:
      return Point3<T>::Create(x, y, z);
   case TOP_RIGHT_NEAR:
      return Point3<T>::Create(x+width, y, z);
   case BOTTOM_LEFT_NEAR:
      return Point3<T>::Create(x, y+height, z);
   case BOTTOM_RIGHT_NEAR:
      return Point3<T>::Create(x+width, y+height, z);
   case TOP_LEFT_FAR:
      return Point3<T>::Create(x, y, z+depth);
   case TOP_RIGHT_FAR:
      return Point3<T>::Create(x+width, y, z+depth);
   case BOTTOM_LEFT_FAR:
      return Point3<T>::Create(x, y+height, z+depth);
   case BOTTOM_RIGHT_FAR:
      return Point3<T>::Create(x+width, y+height, z+depth);
   }
}

//-----------------------------------------------------------------------------

template<typename T> inline bool Box3<T>::ContainsCheck(const Point3<T> &p) const
{
   return (p.x >= x) &&
          (p.x <  x+width) &&
          (p.y >= y) &&
          (p.y <  y+height) &&
          (p.z >= z) &&
          (p.z <  z+depth);
}

//-----------------------------------------------------------------------------

template<typename T> inline bool Box3<T>::OverlapCheck(const Box3<T> &b) const
{
   // Intersects if Max(Left, b.Left) < Min(Right, b.Right)
   //           AND Max(Top, b.Top)   < Min(Bottom, b.Bottom)

   return (x   < (b.x+b.width)) &&
          (b.x < (x+width)) &&
          (y   < (b.y+b.height)) &&
          (b.y < (y+height)) &&
          (z   < (b.z+b.depth)) &&
          (b.z < (z+depth));
}

//-----------------------------------------------------------------------------

template<typename T> Box3<T> Box3<T>::Intersection(const Box3<T> &b) const
{
   Box3<T> newBox;

   newBox.x = std::max(b.x, x);
   newBox.y = std::max(b.y, y);
   newBox.z = std::max(b.z, z);
   T newMaxX = std::min(x+width, b.x+b.width);
   T newMaxY = std::min(y+height, b.y+b.height);
   T newMaxZ = std::min(z+depth, b.z+b.depth);
   newBox.width = newMaxX - newBox.x;
   newBox.height = newMaxY - newBox.y;
   newBox.depth = newMaxZ - newBox.z;

   return newBox;
}

//-----------------------------------------------------------------------------

template<typename T> void Box3<T>::SetToIntersection(const Box3<T> &b)
{
   T newMaxX = std::min(x+width, b.x+b.width);
   T newMaxY = std::min(y+height, b.y+b.height);
   T newMaxZ = std::min(z+depth, b.z+b.depth);
   x = std::max(b.x, x);
   y = std::max(b.y, y);
   z = std::max(b.z, z);
   width = newMaxX - x;
   height = newMaxY - y;
   depth = newMaxZ - z;
}

//-----------------------------------------------------------------------------

template<typename T> Box3<T> Box3<T>::Union(const Box3<T> &b) const
{
   Box3<T> newBox;

   newBox.x = std::min(b.x, x);
   newBox.y = std::min(b.y, y);
   newBox.z = std::min(b.z, z);
   T newMaxX = std::max(x+width, b.x+b.width);
   T newMaxY = std::max(y+height, b.y+b.height);
   T newMaxZ = std::max(z+depth, b.z+b.depth);
   newBox.width = newMaxX - newBox.x;
   newBox.height = newMaxY - newBox.y;
   newBox.depth = newMaxZ - newBox.z;

   return newBox;
}

//-----------------------------------------------------------------------------

template<typename T> void Box3<T>::SetToUnion(const Box3<T> &b)
{
   if(b.EmptyCheck()) return;

   if(EmptyCheck()) *this = b;
   else
   {
      T newMaxX = std::max(x+width, b.x+b.width);
      T newMaxY = std::max(y+height, b.y+b.height);
      T newMaxZ = std::max(z+depth, b.z+b.depth);
      x = std::min(b.x, x);
      y = std::min(b.y, y);
      z = std::min(b.z, z);
      width = newMaxX - x;
      height = newMaxY - y;
      depth = newMaxZ - z;
   }
}

//-----------------------------------------------------------------------------

template<typename T> Box3<T> Box3<T>::Union(const Point3<T> &p) const
{
   Box3<T> newBox = *this;

   newBox.x = std::min(x, p.x);
   newBox.y = std::min(y, p.y);
   newBox.z = std::min(z, p.z);
   T newMaxX = std::max(x+width, p.x);
   T newMaxY = std::max(y+height, p.y);
   T newMaxZ = std::max(z+depth, p.z);
   newBox.width = (newMaxX - newBox.x);
   newBox.height = (newMaxY - newBox.y);
   newBox.depth = (newMaxZ - newBox.z);

   return newBox;
}

//-----------------------------------------------------------------------------

template<typename T> void Box3<T>::SetToUnion(const Point3<T> &p)
{
   T newMaxX = std::max(x+width, p.x);
   T newMaxY = std::max(y+height, p.y);
   T newMaxZ = std::max(z+depth, p.z);
   x = std::min(x, p.x);
   y = std::min(y, p.y);
   z = std::min(z, p.z);
   width = (newMaxX - x);
   height = (newMaxY - y);
   depth = (newMaxZ - z);
}

typedef Box3<int> Box3I;
typedef Box3<float> Box3F;

/// @}
//-----------------------------------------------------------------------------
} // namespace Webfoot
//-----------------------------------------------------------------------------

#endif //#ifndef __FROG__BOX3_H__
 
