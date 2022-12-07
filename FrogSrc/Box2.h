#ifndef __FROG__BOX2_H__
#define __FROG__BOX2_H__

#include "FrogMemory.h"
#include <algorithm>
#include "Point2.h"
#include "TypeData.h"
#include "Utility.h"

//-----------------------------------------------------------------------------

namespace Webfoot
{
//-----------------------------------------------------------------------------

/// \addtogroup DataTypes
/// @{

/// Box2 is a template struct to represent a region (rectangle) in 2D space
template<typename T> class Box2
{
public:
   /// position (inclusive) coordinate for the left (minimum) side of the region.
   T x;
   /// position (inclusive) coordinate for the top (minimum) side of the region.
   T y;
   /// width of the region.  The difference between the maximum x and x.
   T width;
   /// height of the region.  The difference between the maximum y and x.
   T height;

   /// List of the corners of the box for use with CornerGet.
   enum Corner
   {
      TOP_LEFT,
      TOP_RIGHT,
      BOTTOM_LEFT,
      BOTTOM_RIGHT
   };

   /// Method to create a Box2 from a Box2 of a different type.
   template<typename S>
   static Box2 Create(const Box2<S>& a)
   {
      Box2 b = {(T)a.x, (T)a.y, (T)a.width, (T)a.height};
      return b;
   }

   static Box2 Create(T _x, T _y, T _width, T _height) { Box2 b = { _x, _y, _width, _height }; return b; }
   static Box2 Create(const Point2<T>& aMin, const Point2<T>& aMax)
   {
      Box2 b = { aMin.x, aMin.y, aMax.x - aMin.x, aMax.y - aMin.y }; return b;
   }

   /// Set the box to the contents of the rect.
   void Set(T _x, T _y, T _width, T _height) { x = _x; y = _y; width = _width; height = _height; }
   void Set(const Point2<T> &aMin, const Point2<T> &aMax)
   {
      x = aMin.x; y = aMin.y; width = aMax.x - aMin.x; height = aMax.y - aMin.y;
   }

   typename TypeData<T>::MathematicalUpcast WidthGet() const   { return width; }
   typename TypeData<T>::MathematicalUpcast HeightGet() const  { return height; }

   Point2<T> SizeGet() const                                   { return Point2<T>::Create(width, height); }
   typename TypeData<T>::MathematicalUpcast AreaGet()  const   { return WidthGet() * HeightGet(); }

   bool     EmptyCheck()      const                { return ((width == 0) || (height == 0)); }
   void     EmptySet()                          { x = y = width = height = 0; }

   void     WidthSet(T _width)                  { width = _width; }
   void     HeightSet(T _height)                { height = _height; }
   void     SizeSet(T _width, T _height)        { WidthSet(_width); HeightSet(_height);}
   void     SizeSet(const Point2<T> &size)      { width = size.x; height = size.y; }

   /// Set x and y without affecting width and height.
   void PositionSet(const Point2<T>& newPosition) { x = newPosition.x; y = newPosition.y; }

   /// Return the near corner of the box.
   Point2<T> MinGet() const                     { return Point2<T>::Create(x, y); }
   /// Set the near x coordinate of the box without changing the far x coordinate.
   /// This may change the width.
   void MinXSet(const T& minX)                  { width += (x - minX); x = minX; }
   /// Set the near y coordinate of the box without changing the far y coordinate.
   /// This may change the height.
   void MinYSet(const T& minY)                  { height += (y - minY); y = minY; }
   /// Set the near corner of the box without changing the far corner.
   /// This may change the width and height.
   void MinSet(const Point2<T>& _min)           { MinXSet(_min.x); MinYSet(_min.y); }

   /// Return the far x coordinate of the box.
   typename TypeData<T>::MathematicalUpcast MaxXGet() const { return x+width; }
   /// Return the far y coordinate of the box.
   typename TypeData<T>::MathematicalUpcast MaxYGet() const { return y+height; }
   /// Return the far corner of the box
   Point2<T> MaxGet() const { return Point2<T>::Create(x+width, y+height); }

   /// Set the far x coordinate of the box without changing the near x coordinate.
   void MaxXSet(const T& maxX)                  { width = maxX-x; }
   /// Set the far y coordinate of the box without changing the near y coordinate.
   void MaxYSet(const T& maxY)                  { height = maxY-y; }
   /// Set the far corner of the box without changing the near corner.
   void MaxSet(const Point2<T>& _max)           { MaxXSet(_max.x); MaxYSet(_max.y); }

   /// Return true if the height and width are non-negative.
   bool     ValidCheck()      const                { return ((width >= 0) && (height >= 0)); }

   Point2<T>   MidPointGet()  const             { return Point2<T>::Create(x + (width/2), y + (height/2)); }

   // Translation operators
   Box2     operator+(const Point2<T> &t) const       { return Create(x+t.x, y+t.y, width, height); }
   Box2     operator-(const Point2<T> &t) const       { return Create(x-t.x, y-t.y, width, height); }
   void     operator+=(const Point2<T> &t)            { x += t.x; y += t.y; }
   void     operator-=(const Point2<T> &t)            { x -= t.x; y -= t.y; }

   // Scale operators
   Box2     operator*(const Point2<T> &t) const       { return Create(x*t.x, y*t.y, width*t.x, height*t.y); }
   Box2     operator/(const Point2<T> &t) const       { return Create(x/t.x, y/t.y, width/t.x, height/t.y); }
   void     operator*=(const Point2<T> &t)            { x *= t.x; y *= t.y; width *= t.x; height *= t.y; }
   void     operator/=(const Point2<T> &t)            { x /= t.x; y /= t.y; width /= t.x; height /= t.y; }
   Box2     operator*(const T &t) const       { return Create(x*t, y*t, width*t, height*t); }
   Box2     operator/(const T &t) const       { return Create(x/t, y/t, width/t, height/t); }
   void     operator*=(const T &t)            { x *= t; y *= t; width *= t; height *= t; }
   void     operator/=(const T &t)            { x /= t; y /= t; width /= t; height /= t; }

   /// Bit shift the position to the right
   Box2     operator>>(int i) const                   { return Create(x>>i, y>>i, width, height); }
   /// Bit shift the position to the left
   Box2     operator<<(int i) const                   { return Create(x<<i, y<<i, width, height); }

   /// Array access
   const typename TypeData<T>::MathematicalUpcast& operator[](int n) const { return (&x)[n]; }
         typename TypeData<T>::MathematicalUpcast& operator[](int n)       { return (&x)[n]; }

   inline bool operator==( const Box2& b ) const
   {
      return((x == b.x) && (y == b.y) && (width == b.width) && (height == b.height));
   }
   inline bool operator!=( const Box2& b ) const
   {
      return((x != b.x) || (y != b.y) || (width != b.width) || (height != b.height));
   }

   /// Return a point for the given corner.
   inline Point2<T> CornerGet(Corner corner) const;
   
   /// Return the box expanded in all directions by r units.
   friend Box2 Expand(const Box2 &b, int r)           { return Create(b.x - r, b.y - r, b.width + (r*2), b.height + (r*2)); }
   
   /// Return true if this box contains the given box.
   inline bool ContainsCheck(const Box2 &b) const          { return x <= b.x && 
                                                               y <= b.y &&
                                                               x+width >= b.x+b.width &&
                                                               y+height >= b.y+b.height; }

   /// Tests if a point is within the box.
   inline bool ContainsCheck(const Point2<T> &p) const;
   /// Tests if a point is within the box.
   inline bool operator&(const Point2<T> &p) const { return ContainsCheck(p); }
   /// Tests if a point is within the box.
   inline bool operator%(const Point2<T> &p) const { return ContainsCheck(p); }

   /// Tests if two boxes overlap
   inline bool OverlapCheck(const Box2 &b) const;
   /// Tests if two boxes overlap
   inline bool operator%(const Box2 &b) const { return OverlapCheck(b); }

   /// Returns the intersection of two boxes 
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   Box2 Intersection(const Box2 &b) const;
   /// Returns the intersection of two boxes 
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   Box2 operator&(const Box2 &b) const { return Intersection(b); }

   /// Set this box to the intersection of it and the given box.
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   void SetToIntersection(const Box2 &b);
   /// Set this box to the intersection of it and the given box.
   /// !! WARNING !! THIS *ASSUMES* THAT THE BOXES *DO* OVERLAP!
   /// IF IT IS NOT DETERMINED WHETHER THEY DO OR NOT, THEN THE RESULT
   /// SHOULD BE TESTED WITH ValidCheck()
   void operator&=(const Box2 &b) { SetToIntersection(b); }

   /// Returns the union of this box with another - assumes neither is empty
   Box2 Union(const Box2 &b) const;
   /// Returns the union of two boxes - assumes neither is empty
   Box2 operator|(const Box2 &b) const { return Union(b); }

   /// Set this box to the union of it and the given box.
   void SetToUnion(const Box2 &b);
   /// Set this box to the union of it and the given box.
   void operator|=(const Box2 &b) { SetToUnion(b); }

   /// Returns a copy of the box expanded to include the given point.
   Box2 Union(const Point2<T> &p) const;
   /// Returns a copy of the box expanded to include the given point.
   Box2 operator|(const Point2<T> &p) const { return Union(p); }

   /// Expand the box to include the given point
   void SetToUnion(const Point2<T> &p);
   /// Expand the box to include the given point
   void operator|=(const Point2<T> &p) { SetToUnion(p); }

   /// Return the point in the box that is closest to the given point.  Beware
   /// that this treats the right and bottom bounds inclusively.  For example,
   /// if the box is (0, 0, 1024, 768), this could return (1024, 0).
   Point2<T> ClosestPointGet(const Point2<T> &p);

   /// Given another box and a direction, return how much of the relevant sides
   /// are lined up.  For example, start with two squares, A and B, with
   /// 10-unit-long sides.  A is above B and shifted 3 units to the left.  If
   /// you called A.EdgeProjectionOverlapGet(B, DIRECTION_UP) or
   /// A.EdgeProjectionOverlapGet(B, DIRECTION_DOWN), it would return 7.
   /// Calling with any other direction would return 0.  This only works for
   /// cardinal directions.
   T EdgeProjectionOverlapGet(const Box2<T>& other, Direction direction);
   
   /// Return true if the midpoint of this box is generally in the given
   /// direction of the other box.  This only works with cardinal directions.
   /// If neither midpoint is farther in the given direction, return false.
   bool OnSideCheck(const Box2<T>& other, Direction direction);
   /// Return true if this box is fully on the given side of the other box.
   /// For example, if you had two squares, A and B, where A was fully above B,
   /// calling A.FullyOnGivenSideCheck(B, DIRECTION_UP) would return true and
   /// A.FullyOnGivenSideCheck(B, DIRECTION_DOWN) would return false.  This only
   /// works for cardinal directions.  This is inclusive in the sense that if
   /// the boxes are touching, it would still return true.
   bool FullyOnSideCheck(const Box2<T>& other, Direction direction);
};

//-----------------------------------------------------------------------------

template<typename T> Point2<T> Box2<T>::CornerGet(Corner corner) const
{
   switch(corner)
   {
   default:
   case TOP_LEFT:
      return Point2<T>::Create(x, y);
   case TOP_RIGHT:
      return Point2<T>::Create(x+width, y);
   case BOTTOM_LEFT:
      return Point2<T>::Create(x, y+height);
   case BOTTOM_RIGHT:
      return Point2<T>::Create(x+width, y+height);
   }
}

//-----------------------------------------------------------------------------

template<typename T> inline bool Box2<T>::ContainsCheck(const Point2<T> &p) const
{
   return (p.x >= x) &&
          (p.x <  x+width) &&
          (p.y >= y) &&
          (p.y <  y+height);
}


//-----------------------------------------------------------------------------

template<typename T> inline bool Box2<T>::OverlapCheck(const Box2<T> &b) const
{
   // Intersects if Max(Left, b.Left) < Min(Right, b.Right)
   //           AND Max(Top, b.Top)   < Min(Bottom, b.Bottom)

   return (x   < (b.x+b.width)) &&
          (b.x < (x+width)) &&
          (y   < (b.y+b.height)) &&
          (b.y < (y+height));
}


//-----------------------------------------------------------------------------

template<typename T> Box2<T> Box2<T>::Intersection(const Box2<T> &b) const
{
   Box2<T> newBox;

   newBox.x = std::max(b.x, x);
   newBox.y = std::max(b.y, y);
   T newMaxX = std::min(x+width, b.x+b.width);
   T newMaxY = std::min(y+height, b.y+b.height);
   newBox.width = newMaxX - newBox.x;
   newBox.height = newMaxY - newBox.y;

   return newBox;
}

//-----------------------------------------------------------------------------

template<typename T> void Box2<T>::SetToIntersection(const Box2<T> &b)
{
   T newMaxX = std::min(x+width, b.x+b.width);
   T newMaxY = std::min(y+height, b.y+b.height);
   x = std::max(b.x, x);
   y = std::max(b.y, y);
   width = newMaxX - x;
   height = newMaxY - y;
}

//-----------------------------------------------------------------------------

template<typename T> Box2<T> Box2<T>::Union(const Box2<T> &b) const
{
   Box2<T> newBox;

   newBox.x = std::min(b.x, x);
   newBox.y = std::min(b.y, y);
   T newMaxX = std::max(x+width, b.x+b.width);
   T newMaxY = std::max(y+height, b.y+b.height);
   newBox.width = newMaxX - newBox.x;
   newBox.height = newMaxY - newBox.y;

   return newBox;
}


//-----------------------------------------------------------------------------

template<typename T> void Box2<T>::SetToUnion(const Box2<T> &b)
{
   if(b.EmptyCheck()) return;

   if(EmptyCheck()) *this = b;
   else
   {
      T newMaxX = std::max(x+width, b.x+b.width);
      T newMaxY = std::max(y+height, b.y+b.height);
      x = std::min(b.x, x);
      y = std::min(b.y, y);
      width = newMaxX - x;
      height = newMaxY - y;
   }
}

//-----------------------------------------------------------------------------

template<typename T> Box2<T> Box2<T>::Union(const Point2<T> &p) const
{
   Box2<T> newBox = *this;

   newBox.x = std::min(x, p.x);
   newBox.y = std::min(y, p.y);
   T newMaxX = std::max(x+width, p.x);
   T newMaxY = std::max(y+height, p.y);
   newBox.width = (newMaxX - newBox.x);
   newBox.height = (newMaxY - newBox.y);

   return newBox;
}

//-----------------------------------------------------------------------------

template<typename T> void Box2<T>::SetToUnion(const Point2<T> &p)
{
   T newMaxX = std::max(x+width, p.x);
   T newMaxY = std::max(y+height, p.y);
   x = std::min(x, p.x);
   y = std::min(y, p.y);
   width = (newMaxX - x);
   height = (newMaxY - y);
}

//-----------------------------------------------------------------------------

template<typename T> Point2<T> Box2<T>::ClosestPointGet(const Point2<T> &p)
{
   Point2<T> closest;

   // Determine the closest point one dimension at a time.
   if(p.x < x)
      closest.x = x;
   else
   {
      T maxX = MaxXGet();
      if(p.x > maxX)
         closest.x = maxX;
      else
         closest.x = p.x;
   }

   if(p.y < y)
      closest.y = y;
   else
   {
      T maxY = MaxYGet();
      if(p.y > maxY)
         closest.y = maxY;
      else
         closest.y = p.y;
   }

   return closest;
}

//-----------------------------------------------------------------------------

template<typename T> T Box2<T>::EdgeProjectionOverlapGet(const Box2<T>& other, Direction direction)
{
   T thisMin;
   T thisMax;
   T otherMin;
   T otherMax;
   if((direction == DIRECTION_UP) || (direction == DIRECTION_DOWN))
   {
      thisMin = x;
      thisMax = x + width;
      otherMin = other.x;
      otherMax = other.x + other.width;
   }
   else if((direction == DIRECTION_LEFT) || (direction == DIRECTION_RIGHT))
   {
      thisMin = y;
      thisMax = y + height;
      otherMin = other.y;
      otherMax = other.y + other.height;
   }
   else
   {
      return (T)0;
   }

   // Make sure there's some overlap.
   if((thisMin > otherMax) || (thisMax < otherMin))
      return (T)0;

   T mergedMin = std::max(thisMin, otherMin);
   T mergedMax = std::min(thisMax, otherMax);

   return mergedMax - mergedMin;
}

//-----------------------------------------------------------------------------

template<typename T> bool Box2<T>::OnSideCheck(const Box2<T>& other, Direction direction)
{
   Point2<T> thisMidPoint = MidPointGet();
   Point2<T> otherMidPoint = other.MidPointGet();
   if(direction == DIRECTION_UP)
      return thisMidPoint.y < otherMidPoint.y;
   else if(direction == DIRECTION_DOWN)
      return thisMidPoint.y > otherMidPoint.y;
   else if(direction == DIRECTION_LEFT)
      return thisMidPoint.x < otherMidPoint.x;
   else if(direction == DIRECTION_RIGHT)
      return thisMidPoint.x > otherMidPoint.x;
   else
      return false;
} 

//-----------------------------------------------------------------------------

template<typename T> bool Box2<T>::FullyOnSideCheck(const Box2<T>& other, Direction direction)
{
   if(direction == DIRECTION_UP)
      return MaxYGet() <= other.y;
   else if(direction == DIRECTION_DOWN)
      return y >= other.MaxYGet();
   else if(direction == DIRECTION_LEFT)
      return MaxXGet() <= other.x;
   else if(direction == DIRECTION_RIGHT)
      return x >= other.MaxXGet();
   else
      return false;
}

//-----------------------------------------------------------------------------

typedef Box2<int> Box2I;
typedef Box2<float> Box2F;

/// @}
//-----------------------------------------------------------------------------
} // namespace Webfoot
//-----------------------------------------------------------------------------

#endif //#ifndef __FROG__BOX2_H__
