#ifndef __FROG__DUCK__OPENGL__SCENENODEPROJECTORTEXTUREFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODEPROJECTORTEXTUREFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "ShaderProgramGLSL.h"
#include "Duck/SceneNodeProjectorTexture.h"

namespace Webfoot {
namespace Duck {

/// Expected vertex attribute index for position when using a shared texture
/// projection shader.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_POSITION_VERTEX_ATTRIBUTE_INDEX 0
/// Expected vertex attribute index for normal when using a shared texture
/// projection shader.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_NORMAL_VERTEX_ATTRIBUTE_INDEX 1
/// Expected texture unit index for the texture to be projected.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_PROJECTED_TEXTURE_UNIT_INDEX 0
/// Name of the shared shader uniform for the model matrix.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_MODEL_MATRIX_UNIFORM_NAME "modelMatrix"
/// Name of the shared shader uniform for the view matrix.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_VIEW_MATRIX_UNIFORM_NAME "viewMatrix"
/// Name of the shared shader uniform for the projection matrix.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_PROJECTION_MATRIX_UNIFORM_NAME "projectionMatrix"
/// Name of the shared shader uniform for the normal matrix.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_NORMAL_MATRIX_UNIFORM_NAME "normalMatrix"
/// Name of the shared shader uniform for the matrix that projects from world
/// space to texture space.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_PROJECTOR_MATRIX_UNIFORM_NAME "projectorMatrix"
/// Name of the shared shader uniform for the direction of the projector in
/// world space.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_PROJECTOR_DIRECTION_UNIFORM_NAME "projectorDirection"
/// Name of the shared shader uniform for the position of the camera in world
/// space.
#define DUCK_SCENE_NODE_PROJECTOR_TEXTURE_FORWARD_OPENGL_VIEW_POSITION_UNIFORM_NAME "viewPosition"

//==============================================================================

/// Forward-rendered OpenGL SceneNodeProjectorTexture implementation.
class SceneNodeProjectorTextureForwardOpenGL : public SceneNodeProjectorTexture
{
public:
   static void SharedInit();
   static void SharedDeinit();

   /// Apply the projector to the scene.
   virtual void ApplyToScene();

   /// Return a shared shader for the relatively simple case of projecting a
   /// texture onto a fully opaque surface.
   ShaderProgramGLSL* ProjectorTextureShaderProgramOpaqueGet() { return &shaderProgramOpaque; }

   typedef SceneNodeProjectorTexture Inherited;

protected:
   /// Shared shader for the relatively simple case of projecting a texture
   /// onto a fully opaque surface.
   static ShaderProgramGLSL shaderProgramOpaque;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODEPROJECTORTEXTUREFORWARDOPENGL_H__
