#ifndef __FROG__DUCK__MATERIALFORWARDOPENGL_H__
#define __FROG__DUCK__MATERIALFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "Duck/Material.h"

namespace Webfoot {
namespace Duck {

/// Which vertex attribute index should be used for position data.
#define DUCK_MATERIAL_FORWARD_OPENGL_POSITION_VERTEX_ATTRIBUTE_INDEX 0

//==============================================================================

/// OpenGL implementation of MaterialTexture.
class MaterialTextureForwardOpenGL : public MaterialTexture
{
public:
   /// Prepare to use the associated texture on the given texture unit.
   void TextureUnitSetup(int textureUnitIndex);

   typedef MaterialTexture Inherited;
};

//==============================================================================

/// OpenGL implementation of MaterialInstanceTexture.
class MaterialInstanceTextureForwardOpenGL : public MaterialInstanceTexture
{
public:
   /// Convenience function which forwards the request to the associated
   /// MaterialTextureForwardOpenGL.
   void TextureUnitSetup(int textureUnitIndex);

   typedef MaterialInstanceTexture Inherited;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__MATERIALFORWARDOPENGL_H__
