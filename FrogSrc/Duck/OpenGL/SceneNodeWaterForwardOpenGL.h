#ifndef __FROG__DUCK__OPENGL__SCENENODEWATERFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODEWATERFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "ShaderProgramGLSL.h"
#include "Duck/SceneNodeWater.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Helper object for the different index buffers used to draw a single batch
/// (quadtree node) depending on which of its neighbors (if any) have a lower
/// LOD.
class SceneNodeWaterDrawableForwardOpenGLIndexBuffer
{
public:
   SceneNodeWaterDrawableForwardOpenGLIndexBuffer();
   void Deinit();

   /// OpenGL buffer object for the index buffer.
   GLuint indexBufferID;
   /// Number of triangles to draw when using this index buffer.
   size_t triangleCount;
};

//==============================================================================

/// Helper class for drawing a water surface in Duck.
class SceneNodeWaterDrawableForwardOpenGL : public SceneNodeWaterDrawable
{
public:
   SceneNodeWaterDrawableForwardOpenGL();

   virtual void Init(SceneNodeWater* _sceneNodeWater);
   virtual void Deinit();
   virtual void Draw();
   virtual void DrawDepth(const Matrix44& lightProjectionMatrix,
      const Matrix43& lightViewMatrix, const Frustum& lightFrustum);
   virtual void DrawTextureProjection(SceneNodeProjectorTexture* sceneNodeProjectorTexture,
      const Matrix44& projectorMatrix, const Matrix43& projectorTransform);

   typedef SceneNodeWaterDrawable Inherited;

protected:
   void ShaderProgramsInit();
   void ShaderProgramsDeinit();
   /// Helper function for adding the declarations necessary for the effect
   /// of waves on the surface.
   void ShaderProgramWaveDeclarationsAdd(ShaderProgramGLSL* shaderProgram,
      bool generateTangents);
   /// Helper function for setting the uniforms neeed for the effect of waves
   /// on the surface.
   void WaveUniformsSet(ShaderProgramGLSL* shaderProgram);
   /// Helper function for recursively drawing the quadtree.
   void DrawHelper(SceneNodeWaterQuadtreeNode* quadtreeNode,
      const Frustum& viewFrustum, const Matrix43& worldTransform,
      ShaderProgramGLSL* shaderProgram);
   /// Helper function for recursively drawing the curtains based on the
   /// quadtree.
   void CurtainsDrawHelper(SceneNodeWaterQuadtreeNode* quadtreeNode,
      const Frustum& viewFrustum, const Matrix43& worldTransform);

   /// Shader for drawing the top surface of the water.
   ShaderProgramGLSL* surfaceTopShaderProgram;
   /// Shader for drawing the bottom surface of the water.
   ShaderProgramGLSL* surfaceBottomShaderProgram;
   /// Shader for drawing the curtains and floor.
   ShaderProgramGLSL* curtainShaderProgram;
   /// OpenGL buffer object for the vertex buffer of the surface.
   GLuint surfaceVertexBufferID;
   /// OpenGL buffer objects for different index buffers which can be used to
   /// draw a single batch of the surface, depending on which of its neighbors
   /// (if any) are using a lower LOD.
   Table<SceneNodeWaterDrawableForwardOpenGLIndexBuffer*> surfaceIndexBuffers;
   /// OpenGL buffer object for the vertex buffer of the floor.
   GLuint floorVertexBufferID;
   /// OpenGL buffer object for the index buffer of the floor.
   GLuint floorIndexBufferID;
   /// OpenGL buffer object for the vertex buffer of the curtain.
   GLuint curtainVertexBufferID;
   /// OpenGL buffer object for the index buffer of the curtain.
   GLuint curtainIndexBufferID;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODEWATERFORWARDOPENGL_H__
