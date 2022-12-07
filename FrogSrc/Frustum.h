#ifndef __FROG__FRUSTUM_H__
#define __FROG__FRUSTUM_H__

#include "FrogMemory.h"
#include "Plane.h"
#include "Matrix44.h"

namespace Webfoot {

//==============================================================================

class Frustum
{
public:
   /// Set the frustum based on the given view-projection matrix.  This assumes
   /// OpenGL conventions are being followed.  For example the Z range of the
   /// clip coordinates should be [-1, 1].
   void Set(const Matrix44& viewProjectionMatrix);

   /// Return true if the sphere is at least partially within the frustum.
   bool OverlapCheck(const Sphere& sphere) const;

   /// Bit flags for specifying sides or groups of sides of a frustum.
   enum Side
   {
      SIDE_NEAR = 0x01,
      SIDE_FAR = 0x02,
      SIDE_LEFT = 0x04,
      SIDE_RIGHT = 0x08,
      SIDE_TOP = 0x10,
      SIDE_BOTTOM = 0x20,
      /// Number of sides.  (not an actual side)
      SIDE_COUNT = 6,
      /// Union of all sides.  (not an actual side)
      SIDES_ALL = (SIDE_NEAR | SIDE_FAR | SIDE_LEFT | SIDE_RIGHT | SIDE_TOP | SIDE_BOTTOM)
   };

   /// Planes for the sides of the frustum.
   Plane planes[SIDE_COUNT];
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__FRUSTUM_H__
