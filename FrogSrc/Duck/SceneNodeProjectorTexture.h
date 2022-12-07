#ifndef __FROG__DUCK__SCENENODEPROJECTORTEXTURE_H__
#define __FROG__DUCK__SCENENODEPROJECTORTEXTURE_H__

#include "FrogMemory.h"
#include "Matrix44.h"
#include "TextureManager.h"
#include "Duck/SceneNodeProjector.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// A projector that projects a texture.
class SceneNodeProjectorTexture : public SceneNodeProjector
{
public:
   SceneNodeProjectorTexture();

   virtual void Deinit();

   /// Set which texture should be projected.
   void ProjectorTextureSet(const char* filename);
   /// Set the projection matrix for this projector.  Use OpenGL conventions
   /// as if this was for projecting into a framebuffer.
   void ProjectorMatrixSet(const Matrix44& _projectorMatrix);

   typedef SceneNodeProjector Inherited;

protected:
   /// Helper function for cleaning up the loaded texture.
   void ProjectorTextureClear();

   /// Texture to be projected.
   Texture* projectorTexture;
   /// Matrix provided to this object for projecting the texutre.
   Matrix44 framebufferStyleProjectorMatrix;
   /// Matrix actually used to apply the texture.
   Matrix44 textureStyleProjectorMatrix;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEPROJECTORTEXTURE_H__
