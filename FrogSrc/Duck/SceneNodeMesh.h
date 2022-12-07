#ifndef __FROG__DUCK__SCENENODEMESH_H__
#define __FROG__DUCK__SCENENODEMESH_H__

#include "FrogMemory.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

class Mesh;
class SceneNodeMeshGeometryInstance;
class SceneNodeMeshMorphTarget;
class SubmeshInstance;

/// Name of the mesh to be used for this node.
#define DUCK_SCENE_NODE_MESH_MESH_KEY "Mesh"

//==============================================================================

/// Scene node for displaying a mesh.  By default it simply displays the mesh,
/// but this also includes features for drawing multiple copies of the mesh
/// with different transforms.  This can be useful for more efficiently drawing
/// lots of duplicate objects, like foliage or other detail objects.
class SceneNodeMesh : public SceneNode
{
public:
   SceneNodeMesh();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet();

   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// Refresh the bounding volumes based on the current mesh and geometry
   /// instances.
   void BoundingVolumesRefresh();

   /// To activate geometry instancing, set this to a value greater than 0.
   /// This will be the number of copies drawn.  When done making changes,
   /// call 'GeometryInstancesRefresh'.
   void GeometryInstanceCountSet(int instanceCount);
   /// Reserve space for at least this many geometry instances.
   virtual void GeometryInstanceCountReserve(int instanceReserveCount);
   /// See 'InstanceCountSet'.
   int GeometryInstanceCountGet() { return geometryInstances.SizeGet(); }
   /// Return the instance-specific data for the given instance.  When done
   /// making changes, call 'GeometryInstancesRefresh'.
   SceneNodeMeshGeometryInstance* GeometryInstanceGet(int instanceIndex);
   /// Call this when finished making changes to the set of geometry instances
   /// for the frame.
   virtual void GeometryInstancesRefresh();
   /// For geometry instancing purposes, follow the settings of the given node.
   void GeometryInstanceLeaderSet(SceneNodeMesh* _geometryInstanceLeader) { geometryInstanceLeader = _geometryInstanceLeader; }

   /// Return the morph target with the given name.  Return NULL if
   /// unsuccessful.
   SceneNodeMeshMorphTarget* MorphTargetGet(const char* name);

   /// Return the number of submesh instances.
   int SubmeshInstanceCountGet() { return submeshInstances.SizeGet(); }
   /// Return the given submesh instance.
   SubmeshInstance* SubmeshInstanceGet(int submeshInstanceIndex);

   typedef SceneNode Inherited;

protected:
   /// Helper function for doing a ray intersection test.  Return true if
   /// an intersection is detected during this function call.
   bool IntersectHelper(const Ray3& rayLocalSpace, float& intersectDistanceScaled,
      SceneNodeRayIntersectResult* intersectResult);
   /// Set up the 'submeshInstanceBones' collection of each submesh instance.
   void SubmeshInstancesBonesInit();

   /// Mesh displayed by this node.
   Mesh* mesh;
   /// Union of the bounding sphere of all geometry instances.
   Sphere geometryInstancesBoundingSphere;
   /// True if 'SubmeshInstancesBonesInit' has been called.
   bool submeshInstancesBonesInitCalled;
   /// For geometry instancing purposes, follow the settings of this node.
   SceneNodeMesh* geometryInstanceLeader;
   /// Instance-specific data for the submeshes.
   Table<SubmeshInstance*> submeshInstances;
   /// When using instanced drawing, this provides per-instance data.
   Table<SceneNodeMeshGeometryInstance> geometryInstances;
   /// Morph target information specific to this node.
   Table<SceneNodeMeshMorphTarget*> morphTargets;
};

//==============================================================================

/// When drawing large numbers of nearly identical objects at the same time, you
/// can sometimes use hardware instancing for greater efficiency.  This is not
/// to be confused with 'instance' in the sense of two SceneNodes simply sharing
/// data from the same file.  This is for when a single SceneNode needs to be
/// drawn multiple times with different transforms.
class SceneNodeMeshGeometryInstance
{
public:
   /// Set the local position of the instance relative to the associated node.
   void PositionSet(const Point3F& _position) { position = _position; }
   /// Set the local rotation of the instance relative to the associated node.
   void RotationSet(const Quaternion& _rotation) { rotation = _rotation; }
   /// Set the local scale of the instance relative to the associated node.
   void ScaleSet(const Point3F& _scale) { scale = _scale; }

   /// Return the local transform of the instance relative to the associated
   /// node.
   Matrix43 TransformRelativeGet();
   /// Return the inverse of the local transform of the instance relative to
   /// the associated node.
   Matrix43 TransformInverseRelativeGet();

protected:
   /// Local position of the instance relative to the associated node.
   Point3F position;
   /// Local rotation of the instance relative to the associated node.
   Quaternion rotation;
   /// Local scale of the instance relative to the associated node.
   Point3F scale;
   /// Cached bounding sphere of this instance in the local space of the 
   /// associated SceneNodeMesh.
   Sphere boundingSphereNodeSpaceCached;

   friend class SceneNodeMesh;
};

//==============================================================================

/// Aspects of a morph target that are specific to a SceneNodeMesh.
class SceneNodeMeshMorphTarget
{
public:
   SceneNodeMeshMorphTarget();

   void Init(const char* _name, float _weight);
   void Deinit();

   /// Return the name of this morph target.
   const char* NameGet() { return name; }

   /// Return the influence of this morph target.
   float WeightGet() { return weight; }
   /// Set the influence of this morph target.  '_weight' should be in the
   /// range [0, 1].
   void WeightSet(float _weight) { weight = _weight; }

protected:
   /// Name of the morph target.
   const char* name;
   /// Weight
   float weight;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEMESH_H__
