#ifndef __FROG__DUCK__OPENGL__MESHFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__MESHFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "Duck/Mesh.h"

namespace Webfoot {
namespace Duck {

/// Number of rows of each geometry instance-specific transform matrix.
#define DUCK_MESH_FORWARD_OPENGL_GEOMETRY_INSTANCE_TRANSFORM_ATTRIBUTE_ROW_COUNT 3
/// Number of columns of each geometry instance-specific transform matrix.
#define DUCK_MESH_FORWARD_OPENGL_GEOMETRY_INSTANCE_TRANSFORM_ATTRIBUTE_COLUMN_COUNT 4
/// Size in bytes of the transform data of a single geometry instance.
#define DUCK_MESH_FORWARD_OPENGL_GEOMETRY_INSTANCE_TRANSFORM_BUFFER_STRIDE (DUCK_MESH_FORWARD_OPENGL_GEOMETRY_INSTANCE_TRANSFORM_ATTRIBUTE_ROW_COUNT * \
   DUCK_MESH_FORWARD_OPENGL_GEOMETRY_INSTANCE_TRANSFORM_ATTRIBUTE_COLUMN_COUNT * sizeof(float32))

//==============================================================================

/// OpenGL extension of Mesh
class MeshForwardOpenGL : public Mesh
{
public:
   virtual void Init(Scene* _scene, BSONIterator meshSpecs);

   virtual Submesh* SubmeshCreate();

   typedef Mesh Inherited;
};

//==============================================================================

/// OpenGL extension of Submesh
class SubmeshForwardOpenGL : public Submesh
{
public:
   virtual VertexBuffer* VertexBufferCreate();
   virtual IndexBuffer* IndexBufferCreate();
   virtual SubmeshInstance* SubmeshInstanceCreate();

   typedef Submesh Inherited;
};

//==============================================================================

/// OpenGL extension of VertexBuffer
class VertexBufferForwardOpenGL : public VertexBuffer
{
public:
   VertexBufferForwardOpenGL();

   virtual void Init(BSONIterator vertexBufferSpecs);
   virtual void InitDynamic(size_t _bufferDataSize);
   virtual void Deinit();

   virtual void Refresh();

   typedef VertexBuffer Inherited;

   /// OpenGL buffer object for this buffer.
   GLuint bufferID;
};

//==============================================================================

/// OpenGL extension of IndexBuffer
class IndexBufferForwardOpenGL : public IndexBuffer
{
public:
   IndexBufferForwardOpenGL();

   virtual void Init(BSONIterator indexBufferSpecs);
   virtual void Deinit();

   typedef IndexBuffer Inherited;

   /// OpenGL buffer object for this buffer.
   GLuint bufferID;
};

//==============================================================================

/// OpenGL extension of SubmeshInstance
class SubmeshInstanceForwardOpenGL : public SubmeshInstance
{
public:
   virtual void Init(Submesh* _submesh, SceneNodeMesh* _sceneNodeMesh);
   virtual void Deinit();

   virtual void Draw();
   virtual void DrawDepth(const Matrix44& lightProjectionMatrix,
      const Matrix43& lightViewMatrix, const Frustum& lightFrustum);
   virtual void DrawTextureProjection(SceneNodeProjectorTexture* sceneNodeProjectorTexture,
      const Matrix44& projectorMatrix, const Matrix43& projectorTransform);

   typedef SubmeshInstance Inherited;

protected:
   /// Helper function to Draw and DrawTextureProjection.
   virtual void DrawHelper(MaterialMeshConfiguration* _materialMeshConfiguration);
   /// Helper function to Draw for drawing a single pass.
   void DrawPassHelper(int passIndex, MaterialMeshConfiguration* materialMeshConfiguration, 
      IndexBufferForwardOpenGL* indexBuffer, int geometryInstanceCount);
   /// Helper function for setting up a vertex attribute pointer.
   void VertexAttribPointerHelper(GLuint attributeIndex, VertexAttribute* vertexAttribute,
      VertexBufferForwardOpenGL* vertexBuffer);

   /// Temporary collection for keeping track of which vertex attributes are
   /// being used.  Ordinarily, this would be a local variable, but it's been
   /// moved here to avoid lots of allocation and freeing.
   Table<bool> tempVertexAttributeUsed;
   /// True if the most recent attempt to draw this submesh in regular mode
   /// resulted in a fallback from geometry instancing to multiple draw calls.
   bool previousRegularGeometryInstancingFallback;

   /// True if a warning has been given about this object missing a tangent
   /// attribute.
   bool tangentAttributeMissingWarningGiven;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__MESHFORWARDOPENGL_H__
