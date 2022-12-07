#ifndef __FROG__DUCK__ENVIRONMENTMAPFORWARDOPENGL_H__
#define __FROG__DUCK__ENVIRONMENTMAPFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "Duck/EnvironmentMap.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// OpenGL implementation of EnvironmentMap
class EnvironmentMapForwardOpenGL : public EnvironmentMap
{
public:
   EnvironmentMapForwardOpenGL();

   virtual void Init(Bitmap* _bitmap);
   virtual void Deinit();

   /// Return the ID from OpenGL for this texture.
   GLuint TextureIDGet() { return textureID; }

   typedef EnvironmentMap Inherited;

protected:
   /// OpenGL ID number for the cubemap texture.
   /// This will be zero if the texture is not currently registered with OpenGL.
   GLuint textureID;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__ENVIRONMENTMAPFORWARDOPENGL_H__
