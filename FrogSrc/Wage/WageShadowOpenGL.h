#ifndef __FROG__WAGESHADOWOPENGL_H__
#define __FROG__WAGESHADOWOPENGL_H__

#include "Frog.h"
#include "WageShadowCommon.h"

namespace Webfoot {

//=============================================================================

class WageShadowOpenGL : public WageShadowCommon
{
public:
   /// Draw a face of the cube
   void Draw();

   typedef WageShadowCommon Inherited;
   
protected:
};

//=============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__WAGESHADOWOPENGL_H__
