#ifndef __FROG__WAGECUBEOPENGL_H__
#define __FROG__WAGECUBEOPENGL_H__

#include "Frog.h"
#include "WageCubeCommon.h"

namespace Webfoot {

//=============================================================================

class WageCubeOpenGL : public WageCubeCommon
{
public:
   /// Draw a face of the cube
   virtual void DrawFace(Face face, int v1, int v2, int v3, int v4);

   /// Draw a face of the cube using the given position vertices.
   void DrawFace(Face face, const Point3F* positions);

   typedef WageCubeCommon Inherited;
   
protected:
};

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__WAGECUBEOPENGL_H__
