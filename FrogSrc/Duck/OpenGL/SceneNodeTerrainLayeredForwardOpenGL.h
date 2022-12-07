#ifndef __FROG__DUCK__OPENGL__SCENENODETERRAINLAYEREDFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODETERRAINLAYEREDFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "ShaderProgramGLSL.h"
#include "Duck/SceneNodeTerrainLayered.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Helper class for drawing layered, adaptive terrain in Duck.
class SceneNodeTerrainLayeredDrawableForwardOpenGL : public SceneNodeTerrainLayeredDrawable
{
public:
   SceneNodeTerrainLayeredDrawableForwardOpenGL();

   virtual void Init(SceneNodeTerrainLayered* _sceneNodeTerrainLayered);
   virtual void Deinit();
   virtual void Draw();
   virtual void DrawDepth(const Matrix44& lightProjectionMatrix,
      const Matrix43& lightViewMatrix, const Frustum& lightFrustum);
   virtual void DrawTextureProjection(SceneNodeProjectorTexture* sceneNodeProjectorTexture,
      const Matrix44& projectorMatrix, const Matrix43& projectorTransform);

   virtual SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer* QuadtreeNodeVertexBufferCreate();

   typedef SceneNodeTerrainLayeredDrawable Inherited;

protected:
   void ShaderProgramInit();
   void ShaderProgramDeinit() { shaderProgram.Deinit(); }

   enum
   {
      /// Maximum number of layers.
      LAYER_COUNT_MAX = 5
   };

   /// Helper function for recursively drawing the quadtree.
   void DrawHelper(SceneNodeTerrainLayeredQuadtreeNode* quadtreeNode,
      const Frustum& viewFrustum, const Matrix43& worldTransform, bool forDepth);

   /// Shader for drawing the terrain.
   ShaderProgramGLSL shaderProgram;
   /// Buffer for vertex data.
   void* vertexBuffer;
   /// Size of the vertex buffer data in bytes.
   size_t vertexBufferDataSize;
   /// OpenGL buffer object for the index buffer.
   GLuint indexBufferID;
};

//==============================================================================

/// OpenGL implementation of SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer.
class SceneNodeTerrainLayeredQuadtreeNodeVertexBufferForwardOpenGL : public SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer
{
public:
   SceneNodeTerrainLayeredQuadtreeNodeVertexBufferForwardOpenGL();

   void Init(size_t bufferDataSize);
   virtual void Deinit();

   typedef SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer Inherited;

   /// OpenGL buffer object for this buffer.
   GLuint bufferID;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODETERRAINLAYEREDFORWARDOPENGL_H__
