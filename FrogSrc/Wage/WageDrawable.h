#ifndef __WAGEDRAWABLE_H__
#define __WAGEDRAWABLE_H__

#include "Frog.h"

namespace Webfoot {

//==============================================================================

class WageDrawable
{
public:
   virtual ~WageDrawable() {}
   virtual void Draw() = 0;
   virtual float DepthGet() = 0;

   /// A comparison function for sorting items in ascending order by z-position.
   static bool DepthOrderComparator(WageDrawable*& a, WageDrawable*& b)
   {
      return (a->DepthGet() < b->DepthGet());
   }
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __WAGEDRAWABLE_H__
