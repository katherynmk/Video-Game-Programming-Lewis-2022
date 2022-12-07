#ifndef __FROG__DUCK__OPENGL__SCENENODEMESHFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENENODEMESHFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "FrogOpenGL.h"
#include "Duck/SceneNodeMesh.h"
#include "Duck/OpenGL/MeshForwardOpenGL.h"

namespace Webfoot {
namespace Duck {

//==============================================================================

/// Forward-rendered OpenGL SceneNodeMesh implementation.
class SceneNodeMeshForwardOpenGL : public SceneNodeMesh
{
public:
   SceneNodeMeshForwardOpenGL();

   virtual void Deinit();

   virtual void GeometryInstanceCountReserve(int instanceReserveCount);
   virtual void GeometryInstancesRefresh();

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeMeshForwardOpenGL(); }

   typedef SceneNodeMesh Inherited;

protected:
   /// Maximum number of instances of this mesh which could be drawn right now
   /// without reallocations.
   int geometryInstanceTransformBufferInstanceCapacity;
   /// Buffer in which the transforms for the individual instances are placed.
   void* geometryInstanceTransformBuffer;
   /// OpenGL vertex buffer object for the instance transform data.
   GLuint geometryInstanceTransformBufferID;

   friend class SubmeshInstanceForwardOpenGL;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENENODEMESHFORWARDOPENGL_H__
