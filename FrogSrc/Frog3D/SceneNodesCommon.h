#ifndef __FROG__FROG3D__SCENENODESCOMMON_H__
#define __FROG__FROG3D__SCENENODESCOMMON_H__

#include "FrogMemory.h"
#include "RenderData.h"
#include "AnimationState.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneNodeConstants.h"
#include "Box2.h"
#include "Box3.h"
#include "Table.h"
#include "Texture.h"
#include "Sprite.h"
#include "FileManager.h"
#include "Font.h"
#include <typeinfo>

namespace Webfoot {

struct Circle3;
struct Ray3;

class SceneLoadData;
class CollisionHierarchyRootNode;
struct MeshShapeSubmesh;
struct PhysiqueShapeData;
struct PhysiqueShapeSubmesh;
struct SkeletonSceneNodeAnimationEntry;
class TextureProjectorMaterialSceneNode;
class TextureSceneNode;
struct MaterialMeshConfiguration;

struct IntersectData { };
struct IntersectRecord;

/// No more than this number of map channels is allowed.
#define FROG3D_MAP_CHANNEL_COUNT_MAX 8

/// Type of primitive to draw.
enum PrimitiveType
{
   /// Lists of triangles.
   PRIMITIVE_TYPE_TRIANGLES,
   /// Lists of triangles with additional information for adjacent triangles.
   PRIMITIVE_TYPE_TRIANGLES_WITH_ADJACENCY
};

//=============================================================================

/// This is an interface class for objects which can be detected through
/// ray intersection tests.  It is similar to IntersectProvider, except that
/// this is about reporting results rather than performing the tests.
class IntersectResponder
{
public:
   /// Return the name of the object with which the intersection was detected.
   virtual const char* NameGet() const = 0;
   /// Helper function for getting the surface normal in world space for the
   /// given IntersectRecord.
   virtual Point3F NormalGet(RenderData* renderData, const IntersectRecord& record) = 0;
};

//=============================================================================

/// This object is used when performing ray intersection tests to get details
/// of the test results.  It is also used internally to facilitate the tests
/// themselves.  Be sure to call Reset between consecutive tests that are
/// intended to be separate, and set the initial 'intersectDistance' to the
/// maximum distance over which to check.
struct IntersectRecord
{
   /// True if an intersection was detected.
   bool intersectFound;
   /// Distance from the ray's starting position to the closest detected point
   /// of intersection.  You may need to increase this after resetting if the
   /// default distance is too low.  It should be initialized to the maximum
   /// distance you want to test.
   float intersectDistance;
   /// Object with which the closest intersection was detected.  For example,
   /// this could be a RenderSceneNode.
   IntersectResponder* responder;
   /// Internal data for the specific triangle with which the intersection was
   /// detected.
   const IntersectData* responderData;
   
   IntersectRecord()
   {
      Reset();
   }
   
   /// Prepare for a new intersection test.
   void Reset()
   {
      intersectFound = false;
      // Make this relatively large, but not so huge that precision problems
      // occur on minor calculations.
      intersectDistance = 1024.0f;
      responder = NULL;
      responderData = NULL;
   }
   
   /// Return the normal vector in world space of the point at which the
   /// intersection was detected.
   Point3F NormalGet(RenderData* renderData) const
   {
      if(responder && renderData)
         return responder->NormalGet(renderData, *this);
      else
         return Point3F::Create(0.0f, 0.0f, 1.0f);
   }
   
   /// Return the name of the object with which the intersection was detected.
   const char* NameGet() const
   {
      if(responder)
         return responder->NameGet();
      else
         return NULL;
   }
};

//=============================================================================

struct IntersectNode;

/// Base class for collision hierarchy helper objects for intersection tests
/// with a single element, like a triangle.
struct IntersectElement { };

//=============================================================================

/// This is an interface class for objects which can be detected through
/// ray intersection tests.  It is similar to IntersectResponder, except that
/// this is about performing the tests rather than reporting results.
class IntersectProvider
{
public:
   virtual void Intersect(const Ray3& ray, IntersectRecord& intersectRecord, const IntersectElement* element) = 0;
};

//=============================================================================

/// SceneNode is the base class for many of the objects used to render a scene
/// in Frog3D.  This includes the main trees of objects, but it also includes
/// some delegate objects, like MaterialSceneNode, ShapeSceneNode, and
/// TransformControllerSceneNode.  These delegates work more like properties of
/// nodes in the scene graph rather than like children of those nodes.
///
/// Unlike most objects in Frog, SceneNodes use a reference counting system.
/// Rather than calling Deinit and delete or unloading them with
/// theSceneManager when you're done with them, you use the
/// SceneNode::ReferenceAdd and ReferenceRemove methods.  For example, when a
/// tree of nodes is loaded, the reference count of the root is 1.  If you
/// attach it to the scene, the reference count increases to 2.  When the scene
/// is deinitialized, it will automatically decrement the reference count by 1,
/// which would still leave the reference count at 1.  It needs to get down to
/// zero for that tree of nodes to be unloaded though.  If you want to maintain
/// a pointer to the loaded tree of nodes in the game logic, you can clean it
/// up at the end by calling ReferenceRemove on your pointer and setting it to
/// NULL.  If you don't want to maintain your own pointer to the loaded tree of
/// nodes, you should call ReferenceRemove as soon as you've finished setting
/// it up and attaching it to the scene.  Again, do not explicitly call Deinit
/// or delete on a SceneNode.
class SceneNode
{
public:
   virtual void Init(SceneLoadData* input) { (void)input; referenceCount = 1; parent = NULL; }

   void ReferenceAdd() { referenceCount++; }
   void ReferenceRemove() { if(--referenceCount == 0) { Deinit(); frog_delete this; } }
   size_t ReferenceCountGet() const { return referenceCount; }

   virtual const char* NameGet() const { return ""; }

   /// Recursively move time forward by the amount specified in the RenderData.
   virtual void UpdateEarly(RenderData*, bool parentTransformDirty) { (void)parentTransformDirty; }
   /// Recursively update the scene nodes after all the UpdateEarly calls have been made.
   virtual void Update(RenderData*) { }
   virtual void Render(RenderData*) { }

   /// Perform a recursive ray intersection test.  The direction component of
   /// 'ray' must be normalized.  'instructionRecord' must be provided to see
   /// the results and to facilitate the tests.  See the comments on
   /// 'IntersectRecord' for more instructions on how to perform these tests.
   virtual void Intersect(RenderData* renderData, const Ray3& ray, IntersectRecord& intersectRecord);
   
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode = NULL, bool isConstant=false) { ChildIntersectionCacheGenerate(collisionNode, isConstant); }

   virtual const Matrix43& TransformWorldGet(RenderData* renderData = &RenderData::dummyInstance);
   virtual const Matrix43& TransformWorldInverseGet(RenderData* renderData = &RenderData::dummyInstance);

   virtual size_t ChildCountGet() const { return 0; }
   virtual SceneNode* ChildGet(size_t /*i*/) const { return NULL; }

   SceneNode* NodeNextGet();
   SceneNode* NodeLeftMostGet();

   SceneNode* NodeGetByName(const char* name);

   virtual void DebugPrint(int depth = 0);
      
   SceneNode* ParentGet() const { return parent; }
   void ParentSet(SceneNode* aParent) { parent = aParent; }
   
   void NodeRemove();

   void NodesRemoveAllByType(const std::type_info& type);

   virtual void TransformsInvalidate();

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const { return 0.0f; }

protected:
   /// Helper function for calling UpdateEarly on this node's children.
   void ChildrenUpdateEarly(RenderData* renderData, bool parentTransformDirty);
   /// Helper function for calling Update on this node's children.
   void ChildrenUpdate(RenderData* renderData);

   void ChildrenRender(RenderData* renderData);
   virtual void ChildRemove(SceneNode* child);

   virtual char* DebugPrintLineGet(int depth);
   void DebugPrintChildren(int depth);
   const char* DebugPrintSpacePrefixGet(int depth);

   SceneNode() { referenceCount = 1; parent = NULL; }
   virtual ~SceneNode() {}

   void Init() { referenceCount = 1; parent = NULL; }
   virtual void Deinit();

   void ChildIntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode, bool isConstant);

   SceneNode* parent;

private:
   SceneNode(const SceneNode&);
   void operator=(const SceneNode&);

   virtual SceneNode* NodeNextGet(SceneNode* last);

   size_t referenceCount;

   virtual void SetChildrensParents();
   
   friend class SceneManagerCommon;
};

//=============================================================================

/// MaterialSceneNode is the base class for controlling the way the surface of
/// a mesh is drawn.  It works cooperatively with classes like
/// MeshShapeSceneNode and PhysiqueShapeSceneNode.
class MaterialSceneNode : public SceneNode
{
public:
   /// Return the sub-material with the given ID number.  By default, just
   /// return this material.
   virtual MaterialSceneNode* MaterialGet(RenderData* /*renderData*/, int /*subMaterialId*/, AnimationState* /*materialAnimationState*/) { return this; }
   /// Return the number of passes necessary to draw with this material.
   virtual size_t PassCountGet(RenderData* /*renderData*/) const { return 1; }
   
   // Returns MaterialFlags or-ed together
   virtual void MaterialPrePass(RenderData* renderData, size_t passIndex,
      SceneNode* renderNode, MaterialMeshConfiguration* materialMeshConfiguration, AnimationState* materialAnimationState);
   /// This is called when a pass is completed with the given material.
   virtual void MaterialPostPass(RenderData* /*renderData*/, size_t /*passIndex*/,
      SceneNode* /*renderNode*/, AnimationState* /*materialAnimationState*/) {}

   /// Return the primitive type to use for the given pass of this material.
   virtual PrimitiveType PrimitiveTypeForPassGet(RenderData* renderData, size_t passIndex,
      SceneNode* renderNode) { (void)renderData; (void)passIndex; (void)renderNode; return PRIMITIVE_TYPE_TRIANGLES; }

   /// Return false if there's a chance this material will produce a fragment
   /// at less than full opacity on this frame.
   virtual bool FullyOpaqueCheck(RenderData* /*renderData*/, int /*subMaterialId*/, AnimationState* /*materialAnimationState*/) = 0;

#if PLATFORM_IS_3DS
   // Called on certain platforms to help set up vertex attributes based on the
   // needs of the material.
   virtual void SetupVertexAttributes(MaterialMeshConfiguration* materialMeshConfiguration) { (void)materialMeshConfiguration; }
#endif
};

//=============================================================================

/// ShapeSceneNode is the base class for controlling what to draw for a
/// RenderSceneNode and how to draw it.
class ShapeSceneNode : public SceneNode
{
public:
   virtual void Render(RenderData* renderData, SceneNode* renderNode,
      MaterialSceneNode* material, AnimationState* materialAnimationState) = 0;
   virtual void Intersect(RenderData*, const Ray3&, IntersectRecord&) { }
   virtual void Intersect(RenderData* renderData, IntersectResponder* renderNode, const Ray3& ray, IntersectRecord& intersectRecord) = 0;
   virtual void Intersect(IntersectResponder*, const Ray3&, IntersectRecord&, const IntersectElement*) { }
   virtual const Circle3& BoundingSphereGet(RenderData* renderData) const = 0;
   /// Return an approximation of the axis-aligned bounding box in the local space of the shape.
   virtual const Box3F& BoundingBoxGet(RenderData* renderData) const = 0;
   virtual Point3F NormalGet(RenderData* renderData, const IntersectRecord& record) = 0;
   virtual Point3F NormalGet(const IntersectRecord& /*record*/) { return Point3F::Create(0.0f, 0.0f, 0.0f); }

   // To suppress warnings
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode*, bool) { }
   
   // Actual usefull stuff
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* /*collisionNode*/, SceneNode* /*owner*/) { }
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* /*collisionNode*/, const Matrix43& /*transform*/, IntersectProvider* /*provider*/) { }

   /// Return false if there's a chance this object will produce a fragment
   /// at less than full opacity on this frame.
   virtual bool FullyOpaqueCheck(RenderData* /*renderData*/, MaterialSceneNode* /*material*/, AnimationState* /*materialAnimationState*/) = 0;

   /// Called when a material is assigned to a shape.
   virtual void OnMaterialSet(MaterialSceneNode* material) { (void)material; }

private:
   virtual void Render(RenderData*) { }
};

//=============================================================================

/// PathSceneNode is the base class for nodes which provide a 3D spline.
class PathSceneNode : public SceneNode
{
public:
   virtual void PositionGet(RenderData* renderData, Point3F& position, float percent) = 0;
};

//=============================================================================

/// TransformControllerSceneNode is the base class for objects which control
/// the local transforms of other objects in the scene graph.
class TransformControllerSceneNode : public SceneNode
{
public:
   virtual const Matrix43& TransformLocalGet() { return Matrix43::identity; }
   virtual void TransformLocalSet(const Matrix43&) { }
   virtual bool ConstantCheck() const { return false; }

   virtual void TransformApply(Matrix43& transform, RenderData*) = 0;
   virtual void TransformInverseApply(Matrix43& transform, RenderData*) = 0;
};

//=============================================================================

/// PositionControllerSceneNode is the base class for specifying just the
/// position aspect of a local transform, such as for a
/// PRSTransformControllerSceneNode.
class PositionControllerSceneNode : public SceneNode
{
public:
   virtual void TransformApply(Matrix43& transform, RenderData*) = 0;
   virtual void TransformInverseApply(Matrix43& transform, RenderData*) = 0;
   virtual bool ConstantCheck() const { return false; }
};

//=============================================================================

/// RotationControllerSceneNode is the base class for specifying just the
/// rotation aspect of a local transform, such as for a
/// PRSTransformControllerSceneNode.
class RotationControllerSceneNode : public SceneNode
{
public:
   virtual void TransformApply(Matrix43& transform, RenderData*) = 0;
   virtual void TransformInverseApply(Matrix43& transform, RenderData*) = 0;
   virtual bool ConstantCheck() const { return false; }
};

//=============================================================================

/// ScaleControllerSceneNode is the base class for specifying just the scale
/// aspect of a local transform, such as for a PRSTransformControllerSceneNode.
class ScaleControllerSceneNode : public SceneNode
{
public:
   virtual void TransformApply(Matrix43& transform, RenderData*) = 0;
   virtual void TransformInverseApply(Matrix43& transform, RenderData*) = 0;
   virtual bool ConstantCheck() const { return false; }
};

//=============================================================================

/// ValueControllerSceneNode is the base class for scene nodes which provide a
/// scalar value.  This can be used for a variety of purposes, including
/// weights, individual axes of positions, rotations, and scales.
class ValueControllerSceneNode : public SceneNode
{
public:
   virtual void ValueGet(float& value, RenderData*) = 0;
   virtual bool ConstantCheck() const { return false; }
};

//=============================================================================

/// This represents a node of the scene graph which can have children which are
/// not necessarilly drawn.  They may still be drawn if the object is just
/// using this as a base class.
class NonRenderGroupSceneNode : public SceneNode
{
public:
   NonRenderGroupSceneNode();

   void Init();
   virtual void Init(SceneLoadData* input);
   void Init(const NonRenderGroupSceneNode& node);
   virtual void Deinit();

   virtual const char* NameGet() const { return name; }

   virtual size_t ChildCountGet() const { return children.SizeGet(); }
   virtual SceneNode* ChildGet(size_t i) const { return children[i]; }

   void ChildrenCopy(const NonRenderGroupSceneNode& source);

   void ChildAdd(SceneNode* node);
   virtual void ChildRemove(SceneNode* node);
   /// Remove all children.
   void ChildrenRemove();
   
   void Reset() { ChildrenRemove(); }

   /// Reserve space for this many children.
   void ChildrenReserve(int capacity) { children.Reserve(capacity); }

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

   typedef SceneNode Inherited;

private:
   const char* name;

   virtual SceneNode* NodeNextGet(SceneNode* last);

   Table<SceneNode*> children;
};

//=============================================================================

/// This is a node in the scene graph which can have children which can be
/// drawn.
class GroupSceneNode : public NonRenderGroupSceneNode
{
public:
   virtual void UpdateEarly(RenderData* renderData, bool parentTransformDirty) { ChildrenUpdateEarly(renderData, parentTransformDirty); }
   virtual void Update(RenderData* renderData) { ChildrenUpdate(renderData); }
   virtual void Render(RenderData* renderData) { ChildrenRender(renderData); }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) GroupSceneNode(); }
};

//=============================================================================

/// This is a type of GroupSceneNode which is assumed to have no parent for
/// transform purposes.  Its world transform is locked to identity, so it can't
/// be moved, rotated, or scaled.
class RootSceneNode : public GroupSceneNode
{
public:
   virtual const Matrix43& TransformWorldGet(RenderData*) { return Matrix43::identity; }
   virtual const Matrix43& TransformWorldInverseGet(RenderData*) { return Matrix43::identity; }
};

//=============================================================================

/// TransformNode is an interface for certain methods related to the transforms
/// of scene nodes.  This simplifies working with nodes with custom local
/// transforms, because TransformSceneNode takes a base class type as a
/// template parameter.  That template parameter can be avoided in some cases
/// by using this type instead of TransformSceneNode.
class TransformNode
{
public:
   virtual void TransformControllerSet(TransformControllerSceneNode* newController) = 0;
   virtual TransformControllerSceneNode* TransformControllerGet() const = 0;

   virtual const Matrix43& TransformLocalGet() = 0;
   virtual void TransformLocalSet(const Matrix43& transform) = 0;
   virtual const Matrix43& TransformWorldGet(RenderData* renderData = &RenderData::dummyInstance) = 0;
   virtual const Matrix43& TransformWorldInverseGet(RenderData* renderData = &RenderData::dummyInstance) = 0;
};

//=============================================================================

/// TransformSceneNode is the base class for nodes which have a transform.
/// These nodes delegate their local transform to a
/// TransformControllerSceneNode.  The world transform is usually also affected
/// by the world transform of the parent node, but a transform controller can
/// override this.
///
/// TransformSceneNode uses a caching system for world transforms to avoid
/// redundant, recursive evaluations of these matrices.  In most cases, these
/// transforms are recomputed during the UpdateEarly call.
///
/// This class inherits from a type specified as a template parameter.  This
/// means you can't simply have a pointer to a TransformSceneNode.  Instead,
/// you may need to account for the different possible base classes, assume a
/// specific base class, or use a TransformNode pointer.
template<typename BASE_CLASS> class TransformSceneNode : public BASE_CLASS, public TransformNode
{
public:
   TransformSceneNode();

   void Init();
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   virtual void Update(RenderData*);

   virtual const Matrix43& TransformWorldGet(RenderData* renderData = &RenderData::dummyInstance);
   virtual const Matrix43& TransformWorldInverseGet(RenderData* renderData = &RenderData::dummyInstance);

   virtual const Matrix43& TransformLocalGet() { return transformController->TransformLocalGet(); }
   virtual void TransformLocalSet(const Matrix43& transform) { transformController->TransformLocalSet(transform); TransformsInvalidate(); }
   virtual void TransformControllerSet(TransformControllerSceneNode* newController);
   
   virtual TransformControllerSceneNode* TransformControllerGet() const { return transformController; }
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode=NULL, bool isConstant=false);
   
   virtual void TransformsInvalidate() { worldTransformCacheId = -1; inverseWorldTransformCacheId = -1; BASE_CLASS::TransformsInvalidate(); }

   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID);

   /// Return the object that controls time for the transform.
   AnimationState* TransformAnimationStateGet() { return &transformAnimationState; }

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

protected:
   void Init(const TransformSceneNode& node, bool copyTransformController);
   TransformControllerSceneNode* transformController;
   
private:
   int worldTransformCacheId;
   Matrix43 worldTransform;

   int inverseWorldTransformCacheId;
   Matrix43 inverseWorldTransform;

   AnimationState transformAnimationState;
};

//=============================================================================

/// Update and render the children during a specific range of time.  When that
/// time ends, this node is removed from the scene graph.
class TimeLimitedSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   void Init(float startDelay, float duration);
   virtual void Init(SceneLoadData* input) { assert(false); Inherited::Init(input); }

   virtual void UpdateEarly(RenderData* renderData, bool parentTransformDirty);
   virtual void Update(RenderData* renderData);
   virtual void Render(RenderData* renderData);
   virtual void Intersect(RenderData* renderData, const Ray3& ray, IntersectRecord& intersectRecord);

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

private:
   typedef TransformSceneNode<GroupSceneNode> Inherited;

   /// Keeps track of when the node should begin updating and rendering its
   /// children.
   AnimationState startTimer;
   /// Keeps track of when the node should expire.
   AnimationState timeLimit;
};

//=============================================================================

/// This is a specialization of IntersectResponder for when the test was
/// performed using a CollisionHierachy.  This is necessary, because the way
/// the surface normal is determined is different.
class RenderSceneNodeCollisionHierarchyHelper : public IntersectResponder
{
   virtual Point3F CollisionHierarchyNormalGet(const IntersectRecord& record) = 0;
   virtual Point3F NormalGet(RenderData* /*renderData*/, const IntersectRecord& record) { return CollisionHierarchyNormalGet(record);   }
};

//=============================================================================

/// This is a specialization of IntersectResponder for when the test was
/// performed directly, rather than using a CollisionHierachy.  This is
/// necessary, because the way the surface normal is determined is different.
class RenderSceneNodeCollisionHelper : public IntersectResponder
{
   virtual Point3F CollisionNormalGet(RenderData* renderData, const IntersectRecord& record) = 0;
   virtual Point3F NormalGet(RenderData* renderData, const IntersectRecord& record) { return CollisionNormalGet(renderData, record); }
};

//=============================================================================

/// This is the class for nodes which should be drawn.  The transform of this
/// node is delegated to a TransformControllerSceneNode, its shape is delegated
/// to a ShapeSceneNode, and the appearance of its surface is often delegated
/// to a MaterialSceneNode.  However, not all types of ShapeSceneNodes will
/// make use of the MaterialSceneNode.
class RenderSceneNode : public TransformSceneNode<GroupSceneNode>, 
   public IntersectProvider, public RenderSceneNodeCollisionHelper,
   public RenderSceneNodeCollisionHierarchyHelper
{
public:
   RenderSceneNode();

   void Init();
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   virtual void Update(RenderData*);
   virtual void Render(RenderData* renderData);
   virtual void Intersect(RenderData* renderData, const Ray3& ray, IntersectRecord& intersectRecord);
   virtual void DebugPrint(int depth);

   // Create a new node that references the existing material and shape, but
   // has its own transform.  This new node has to be added to the scene if it
   // is to be seen.
   RenderSceneNode* Clone(bool copyTransformController=true) const;

   virtual const char* NameGet() const { return TransformSceneNode<GroupSceneNode>::NameGet(); }
   virtual Circle3 BoundingSphereGet(RenderData* renderData);

   virtual Point3F CollisionNormalGet(RenderData* renderData, const IntersectRecord& record);
   virtual Point3F CollisionHierarchyNormalGet(const IntersectRecord& record);

   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode=NULL, bool isConstant=false);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) RenderSceneNode(); }

   bool VisibleCheck() const { return visible; }
   void VisibleSet(bool _visible) { visible = _visible; }
   void CollidableSet(bool newCollidable) { collidable = newCollidable; }
   
   ShapeSceneNode* ShapeGet() const { return shape; }
   void ShapeSet(ShapeSceneNode* node);

   MaterialSceneNode* MaterialGet() const { return material; }
   void MaterialSet(MaterialSceneNode* node);

   /// Set the depth-sort priority if this node.  Depth-sorted nodes of
   /// numerically higher priority are drawn after depth-sorted nodes of lower
   /// priority.  Depth-sorted nodes of the same priority are sorted by depth.
   /// The default priority is 0.
   void DepthSortPrioritySet(float _depthSortPriority) { depthSortPriority = _depthSortPriority; }
   /// See 'DepthSortPrioritySet'.
   float DepthSortPriorityGet() { return depthSortPriority; }

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

   /// Return true if view frustum culling should be used with this object.
   bool ViewFrustumCullingEnabledCheck() { return viewFrustumCullingEnabled; }
   /// Set whether view frustum culling should be used with this object.
   void ViewFrustumCullingEnabledSet(bool _viewFrustumCullingEnabled) { viewFrustumCullingEnabled = _viewFrustumCullingEnabled; }

   /// Helper for building and drawing the list of nodes that need depth
   /// sorting.  This is for use by the SceneManager only.
   RenderSceneNode* nextSortedNode;
   /// Helper for sorting nodes by depth.  This is for use by the SceneManager
   /// only.
   float depthSortValue;
   /// Helper for sorting nodes by depth.  This is for use by the SceneManager
   /// only.
   float depthSortPriority;

protected:
   void Init(const RenderSceneNode& node, bool copyTransformController=true);

private:
   bool visible; 
   bool collidable;
   MaterialSceneNode* material;
   ShapeSceneNode* shape;
   /// True if view frustum culling should be used with this object.
   bool viewFrustumCullingEnabled;

   /// Handles timing of the material animation for this node.
   AnimationState materialAnimationState;

   // From IntersectProvider
   virtual void Intersect(const Ray3& ray, IntersectRecord& intersectRecord, const IntersectElement* element);

   typedef TransformSceneNode<GroupSceneNode> Inherited;
};

//=============================================================================

/// This node type provides a 3D spline such that the transform of this node
/// affects the reported path of the spline.
class SplineSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   SplineSceneNode() { path = NULL; }

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void DebugPrint(int depth);

   void SplinePositionGet(RenderData* renderData, Point3F& position, float percent);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) SplineSceneNode(); }

private:
   PathSceneNode* path;
   
   typedef TransformSceneNode<GroupSceneNode> Inherited;
};

//=============================================================================

/// Helper objects can be exported as part of a 3D scene, and this class type
/// is used to represent them at runtime.  They can be useful for specifying a
/// transform to be used by the game, like a starting point for a character.
class HelperSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   virtual void Intersect(RenderData*, const Ray3&, IntersectRecord&) { }
   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) HelperSceneNode(); }

private:
   typedef TransformSceneNode<GroupSceneNode> Inherited;
};

//=============================================================================

/// This controller forces its local transform to also be used as its world
/// transform.  It does NOT inherit the parent node's transform.  The local
/// transform for this controller is just stored as a matrix.
class FixedTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   // Set both to identity
   void Init();
   void Init(const Matrix43& aTransform);

   virtual void TransformLocalSet(const Matrix43& aTransform);

   virtual void TransformApply(Matrix43& aTransform, RenderData*) { aTransform = transform; }
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);

   virtual bool ConstantCheck() const { return true; }

private:
   bool inverseIsValid;

   Matrix43 transform;
   Matrix43 inverseTransform;
};

//=============================================================================

/// This controller simply uses a matrix for the local transform.  This is
/// often the type of TransformControllerSceneNode used by default.
class ConstantTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   void Init();
   void Init(const Matrix43& aTransform);
   virtual void Init(SceneLoadData* input);

   virtual void TransformApply(Matrix43& aTransform, RenderData*) { aTransform = aTransform * transform; }
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);

   virtual const Matrix43& TransformLocalGet() { return transform; }
   virtual void TransformLocalSet(const Matrix43& newTransform) { transform = newTransform; inverseIsValid = false; }
   virtual bool ConstantCheck() const { return true; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) ConstantTransformControllerSceneNode(); }
   
private:
   bool inverseIsValid;

   Matrix43 transform;
   Matrix43 inverseTransform;
};

//=============================================================================

/// This controller causes the +y direction of the object to face the camera
/// and the +z direction of the object to point upward from the camera's
/// perspective.  Its position is determined by a vector, the orientation is
/// determined by the offset from the object to the camera, and the scale is
/// always 1.  Whether or not the transform of the parent affects the world
/// transform of this controller is determined by a setting.  It's often useful
/// when displaying 2D objects, like sprites, text, and flat meshes, in a 3D
/// scene.
class BillboardTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   void Init(const Point3F& _position, bool _relativeToParent = true);
   virtual void Init(SceneLoadData* input);

   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);

   void PositionSet(const Point3F& p) { position = p; }
   const Point3F& PositionGet() const { return position; }

   /// Set to true if 'position' should be relative to the parent, and false if
   /// it is in world coordinates.
   void RelativeToParentSet(bool _relativeToParent) { relativeToParent = _relativeToParent; }
   /// See 'RelativeToParentSet'.
   bool RelativeToParentCheck() { return relativeToParent; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) BillboardTransformControllerSceneNode(); }

private:
   Point3F position;
   /// True if 'position' should be relative to the parent, and false if it is
   /// in world coordinates.
   bool relativeToParent;
};

//=============================================================================

/// This transform controller will rotate the object about the vertical axis to
/// align it to the camera's projection plane.  The object will then roughly
/// face the camera.  This useful for trees, street lights, and anything else
/// that needs to face the camera but remain upright regardless of the camera's
/// pitch.  This is different from a regular billboard, which will rotate
/// about all three axes.
class UprightBillboardTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   void Init(const Point3F& _position);
   virtual void Init(SceneLoadData* input);
   
   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);

   void PositionSet(const Point3F& p) { position = p; }
   const Point3F& PositionGet() const { return position; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) UprightBillboardTransformControllerSceneNode(); }

private:
   Point3F position;
};

//=============================================================================

/// This controller delegates the position, rotation, and scale of the
/// local transform to different objects.
class PRSTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   PRSTransformControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual float ControllerTimeMaxGet() const;

   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) PRSTransformControllerSceneNode(); }

private:
   PositionControllerSceneNode* positionController;
   RotationControllerSceneNode* rotationController;
   ScaleControllerSceneNode* scaleController;
};

//=============================================================================

/// This transform controller determines the local transform using
/// interpolation and an internal sequence of transform matrices.
class SequenceTransformControllerSceneNode : public TransformControllerSceneNode
{
public:  
   SequenceTransformControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual float ControllerTimeMaxGet() const;

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) SequenceTransformControllerSceneNode(); }

private:
   int timeScale;

   size_t numberOfTransforms;
   Matrix43* transformList;
};

//=============================================================================

/// This controller provides a proxy for using a subset of the timeline of
/// another controller.
class TimeSubsetTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   TimeSubsetTransformControllerSceneNode();
   
   void Init(TransformControllerSceneNode* _baseController, float _startTime,
      float _duration);
   virtual void Deinit();

   virtual const Matrix43& TransformLocalGet();
   virtual void TransformLocalSet(const Matrix43&);
   virtual bool ConstantCheck() const;
   virtual float ControllerTimeMaxGet() const;

   virtual void TransformApply(Matrix43& transform, RenderData*);
   virtual void TransformInverseApply(Matrix43& transform, RenderData*);

   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   virtual void Update(RenderData*);

   typedef TransformControllerSceneNode Inherited;

protected:
   /// Use transforms from a subset of the timeline of this node.
   TransformControllerSceneNode* baseController;
   /// Beginning of the range of time to use from 'baseController', in seconds.
   float startTime;
   /// Length of the range of time to use, in seconds.
   float duration;
};

//=============================================================================

/// This controller provides an unchanging position as a vector.
class ConstantPositionControllerSceneNode : public PositionControllerSceneNode
{
public:
   virtual void Init(SceneLoadData* input);

   //TEMPORARY
   virtual void TransformApply(Matrix43& transform, RenderData*) { transform += Point3F::Create(p); transform.flags &= flagMask; }
   virtual void TransformInverseApply(Matrix43& transform, RenderData*) { transform -= Point3F::Create(p); transform.flags &= flagMask; }
   virtual bool ConstantCheck() const { return true; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) ConstantPositionControllerSceneNode(); }

private:
   Point3F p;
   unsigned flagMask;
};

//=============================================================================

/// This controller provides a position and gets the values for the individual
/// axes from other controllers.
class XYZPositionControllerSceneNode : public PositionControllerSceneNode
{
public:
   XYZPositionControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual float ControllerTimeMaxGet() const;

   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) XYZPositionControllerSceneNode(); }

private:
   ValueControllerSceneNode* xController;
   ValueControllerSceneNode* yController;
   ValueControllerSceneNode* zController;
};

//=============================================================================

struct PathPositionData;

/// This controller provides a position based on a spline.
class PathPositionControllerSceneNode : public PositionControllerSceneNode
{
public:
   PathPositionControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual float ControllerTimeMaxGet() const;

   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) PathPositionControllerSceneNode(); }

private:
   ValueControllerSceneNode* percentController;
   
   size_t numberOfNodes;
   PathPositionData* nodes;
   
   Point3F TranslationGet(RenderData* renderData);
};

//=============================================================================

/// This controller provides an unchanging rotation in the form of a matrix.
class ConstantRotationControllerSceneNode : public RotationControllerSceneNode
{
public:
   virtual void Init(SceneLoadData* input);
   
   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual bool ConstantCheck() const { return true; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) ConstantRotationControllerSceneNode(); }

private: 
   Matrix43 transform;
   Matrix43 inverseTransform;
};

//=============================================================================

/// This controller provides a rotation based on Euler angles with the angles
/// applied in the order x, y, and z.  The angles for the individual axes come
/// from separate objects.
class EulerXYZRotationControllerSceneNode : public RotationControllerSceneNode
{
public:
   EulerXYZRotationControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual bool ConstantCheck() const { return true; }
   virtual float ControllerTimeMaxGet() const;

   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) EulerXYZRotationControllerSceneNode(); }

private: 
   ValueControllerSceneNode* xController;
   ValueControllerSceneNode* yController;
   ValueControllerSceneNode* zController;
};

//=============================================================================

/// This controller provides an unchanging 3D scale as a matrix.
class ConstantScaleControllerSceneNode : public ScaleControllerSceneNode
{
public:
   virtual void Init(SceneLoadData* input);
   
   virtual void TransformApply(Matrix43& aTransform, RenderData*);
   virtual void TransformInverseApply(Matrix43& aTransform, RenderData*);
   virtual bool ConstantCheck() const { return true; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) ConstantScaleControllerSceneNode(); }

private: 
   Matrix43 transform;
   Matrix43 inverseTransform;
};

//=============================================================================

/// This controller provides an unchanging scalar value.
class ConstantValueControllerSceneNode : public ValueControllerSceneNode 
{
public:
   virtual void Init(SceneLoadData* input);
   
   virtual void ValueGet(float& value, RenderData*) { value = v; }
   virtual bool ConstantCheck() const { return true; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) ConstantValueControllerSceneNode(); }
   
private: 
   float v;
};

//=============================================================================

struct BezierValueControllerData;

/// This controller provides a scalar value based on a Bezier spline.
class BezierValueControllerSceneNode : public ValueControllerSceneNode 
{
public:
   BezierValueControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void ValueGet(float& value, RenderData*);
   virtual float ControllerTimeMaxGet() const;

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) BezierValueControllerSceneNode(); }

private:
   size_t lastKeyIndex;
   size_t numberOfKeys;
   BezierValueControllerData* keys;
};

//=============================================================================

struct LinearValueControllerData;

/// This controller provides a scalar value by linearly interpolating along a
/// sequence of values.
class LinearValueControllerSceneNode : public ValueControllerSceneNode
{
public:  
   LinearValueControllerSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void ValueGet(float& value, RenderData*);
   virtual float ControllerTimeMaxGet() const;

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) LinearValueControllerSceneNode(); }

private:
   size_t lastKeyIndex;
   size_t numberOfKeys;
   LinearValueControllerData* keys;
};

//=============================================================================

/// Contains data on how morph channel values should be manipulated over time
/// for a single animation.
class MorphAnimationSceneNode : public SceneNode
{
public:
   void Init();
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   /// Return the maximum duration across all the controllers.
   virtual float ControllerTimeMaxGet() const;
   virtual int MorphChannelControllerCountGet() { return morphChannelControllers.SizeGet(); }

   /// Return the value for the given channel.
   virtual float MorphChannelValueGet(RenderData* renderData, int channelIndex);
   
   /// Add the given morph channel controller to the back of the collection. 
   virtual void MorphChannelControllerAdd(ValueControllerSceneNode* controller);
   /// Return the controller for the given channel. 
   virtual ValueControllerSceneNode* MorphChannelControllerGet(int channelIndex);
   
   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) MorphAnimationSceneNode(); }
   
   typedef SceneNode Inherited;

protected:
   /// Collection of controllers that drive the values for the morph channels
   /// over time.  The controllers are listed in the same order as in
   /// the corresponding collection of morph targets.
   Table<ValueControllerSceneNode*> morphChannelControllers;
};

//=============================================================================

/// This object provides a proxy for using a subset of the timeline of
/// another MorphAnimationSceneNode.
class TimeSubsetMorphAnimationSceneNode : public MorphAnimationSceneNode
{
public:
   TimeSubsetMorphAnimationSceneNode();
   
   void Init(MorphAnimationSceneNode* _baseAnimation, float _startTime,
      float _duration);
   virtual void Deinit();

   /// Return the maximum duration across all the controllers.
   virtual float ControllerTimeMaxGet() const;
   virtual int MorphChannelControllerCountGet();

   /// Return the value for the given morph channel.
   virtual float MorphChannelValueGet(RenderData* renderData, int channelIndex);
   
   /// Add the given morph controller to the back of the collection.
   /// Do not call this with TimeSubsetMorphAnimationSceneNode.
   virtual void MorphChannelControllerAdd(ValueControllerSceneNode*) { assert(false); }
   /// Return the controller for the given channel. 
   virtual ValueControllerSceneNode* MorphChannelControllerGet(int channelIndex);
   
   typedef TimeSubsetMorphAnimationSceneNode Inherited;

protected:
   /// Underlying animation of which a subset is used by this object.
   MorphAnimationSceneNode* baseAnimation;
   /// Beginning of the range of time to use from 'baseAnimation', in seconds.
   float startTime;
   /// Length of the range of time to use, in seconds.
   float duration;
};

//=============================================================================

/// Caches some values used in ray-triangle intersection tests for a single
/// triangle.
struct MeshIntersectCache : public IntersectData, public IntersectElement
{
   /// Normal of the triangle.
   Point3F n;
   /// Signed distance from the local origin to the plane of the triangle along
   /// the direction of the normal.
   float aOffset;

   // sVector, sOffset, tVector, and tOffset can be used to quickly compute the
   // ST coordinates of a point in the same plane as the triangle.
   Point3F sVector;
   float sOffset;

   Point3F tVector;
   float tOffset;
   
   void Prepare(const Point3F v[3]);
   void Prepare(const Point3F& v0, const Point3F& v1, const Point3F& v2);
   void Intersect(const Ray3& ray, IntersectRecord& record, IntersectResponder* responder) const;
   static void Intersect(const Ray3& ray, IntersectRecord& record, const MeshIntersectCache* intersectCache, size_t intersectCacheLength, IntersectResponder* responder);
};

//=============================================================================

/// Describes a single channel of vertex data.
struct VertexAttribute
{
   /// Offset from the beginning of a vertex to the data for this attribute.
   size_t offset;
   /// Whether the elements are float32, uint8, etc.
   VertexAttributeDataType dataType;
   /// Number of elements in the associated attribute.
   uint8 dataSize;
};

//=============================================================================

/// Part of a mesh to be drawn with a single native draw call.  This includes
/// vertex data, index data, and a description of the vertex attributes.  For
/// now, assume that position and normal data are included as Point3Fs at
/// the beginning of each vertex.
struct MeshShapeSubmeshCommon
{
   MeshShapeSubmeshCommon(SceneLoadData* input);
   ~MeshShapeSubmeshCommon();

   /// Number of vertices in the collection.
   uint32 numberOfVertices;
   /// Size of a singe vertex in bytes.
   size_t vertexSize;
   /// Submaterial number for this set of faces.
   unsigned char materialId;
   /// Vertex buffer.
   void* vertexData;
   /// Number of faces to draw for this submesh.
   uint32 numberOfFaces;
   /// Whether the indices are uint16s, uint32s, etc.
   IndexDataType indexDataType;
   /// Index buffer.
   void* indexData;
   /// Whether the indices are uint16s, uint32s, etc.
   IndexDataType indexDataWithAdjacencyType;
   /// Index buffer for triangles with adjacency.
   void* indexDataWithAdjacency;
   /// Descriptions of the individual attributes for the map channels.
   Table<VertexAttribute> mapChannelVertexAttributes;
};

//-----------------------------------------------------------------------------

struct MeshShapeData
{
   size_t numberOfSubmeshes;
   MeshShapeSubmesh** submeshes;

   Circle3 boundingSphere;
   /// Axis-aligned bounding box in the local space of the mesh.
   Box3F boundingBox;
   
   size_t intersectCacheLength;
   MeshIntersectCache* intersectCache;

   MeshShapeData(SceneLoadData* input);
   ~MeshShapeData();
   
   void IntersectCacheBuild();
   /// Helper function for processing all the faces in a given submesh.
   template <typename IndexType>
   void IntersectionCacheBuildHelper(MeshIntersectCache** p, const MeshShapeSubmeshCommon* submesh);

   void Intersect(IntersectResponder* responder, const Ray3& ray, IntersectRecord& intersectRecord);   
   void Intersect(IntersectResponder* responder, const Ray3& ray, IntersectRecord& record, const IntersectElement* element) const;

   void CollisionHierarchyDataGenerate(CollisionHierarchyRootNode* collisionNode,
      const Matrix43& transform, IntersectProvider* provider);
   /// Helper function for processing all the faces in a given submesh.
   template <typename IndexType>
   void CollisionHierarchyDataGenerateHelper(CollisionHierarchyRootNode* collisionNode,
      const Matrix43& transform, IntersectProvider* provider,
      MeshIntersectCache** p, const MeshShapeSubmeshCommon* submesh);
};

//-----------------------------------------------------------------------------

template <typename IndexType>
void MeshShapeData::IntersectionCacheBuildHelper(MeshIntersectCache** p, const MeshShapeSubmeshCommon* submesh)
{
   IndexType* indexBufferPosition = (IndexType*)submesh->indexData;
   
   for(size_t faceIndex = 0; faceIndex < submesh->numberOfFaces; faceIndex++)
   {
      // Get the indices of this face.
      IndexType index0, index1, index2;
      index0 = *indexBufferPosition; indexBufferPosition++;
      index1 = *indexBufferPosition; indexBufferPosition++;
      index2 = *indexBufferPosition; indexBufferPosition++;

      // Extract the vertex positions from the vertex buffer.  Use
      // memcpy in case the current platform can't handle misaligned
      // usage of floats.  This is assuming the position is the first
      // part of each vertex.
      Point3F p0, p1, p2;
      void* p0Source = (void*)((size_t)submesh->vertexData + (index0 * submesh->vertexSize));
      void* p1Source = (void*)((size_t)submesh->vertexData + (index1 * submesh->vertexSize));
      void* p2Source = (void*)((size_t)submesh->vertexData + (index2 * submesh->vertexSize));
      memcpy((void*)&p0, p0Source, sizeof(Point3F));
      memcpy((void*)&p1, p1Source, sizeof(Point3F));
      memcpy((void*)&p2, p2Source, sizeof(Point3F));

      (*p)->Prepare(p0, p1, p2);
      ++(*p);
   }
}

//=============================================================================

/// This object is used to draw a static mesh with a RenderSceneNode.  The
/// appearance of the surface is determined by a MaterialSceneNode.  The mesh
/// is divided internally into different submeshes based mainly on which parts
/// of the mesh need to be drawn with different submaterials.  It only contains
/// the code common to all platforms, so it can't be used directly.  Use
/// MeshShapeSceneNode instead.
class MeshShapeSceneNodeCommon : public ShapeSceneNode
{
public:
   MeshShapeSceneNodeCommon();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual const Circle3& BoundingSphereGet(RenderData* renderData) const;
   virtual const Box3F& BoundingBoxGet(RenderData* renderData) const;
   virtual void Intersect(RenderData* renderData, IntersectResponder* responder, const Ray3& ray, IntersectRecord& intersectRecord);
   virtual void Intersect(IntersectResponder* responder, const Ray3& ray, IntersectRecord& intersectRecord, const IntersectElement* element);
   virtual Point3F NormalGet(RenderData* renderData, const IntersectRecord& record);
   virtual Point3F NormalGet(const IntersectRecord& record);

   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode, SceneNode* owner);
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode, const Matrix43& transform, IntersectProvider* provider);

   virtual bool FullyOpaqueCheck(RenderData* renderData, MaterialSceneNode* material, AnimationState* materialAnimationState);

protected:
   MeshShapeData* mesh;
};

//=============================================================================

/// Specifies the effect of a given morph target on a given vertex at maximum influence.
struct MorphVertexOffset
{
   /// Index of the vertex in the mesh.
   uint32 index;
   /// Offset from the rest position of the vertex to the morphed position.
   Point3F offset;
};

//=============================================================================

/// Stores animation-independent, submesh-independent data for a given mesh
/// morph target.
struct MorphTarget
{
   /// When using progressive morph, this is the fraction of full weight of
   /// the associated morph channel at which this target is at full
   /// intensity.
   float progress;
};

//=============================================================================

/// Stores animation-independent, submesh-independent data for a given mesh
/// morph channel.
struct MorphChannel
{
   /// Name of the channel.
   const char* name;

   /// Collection of targets for this channel.
   Table<MorphTarget> morphTargets;
};

//=============================================================================

/// Stores animation-independent, submesh-dependent data for a given mesh
/// morph target.
struct SubmeshMorphTarget
{
   /// How the vertices should be changed for this target.
   Table<MorphVertexOffset> vertexOffsets;
};

//=============================================================================

/// Stores animation-independent, submesh-dependent data for a given mesh
/// morph channel.
struct SubmeshMorphChannel
{
   /// Collection of targets for this channel.
   Table<SubmeshMorphTarget> submeshMorphTargets;
};

//==============================================================================

/// Helper object to SkeletonSceneNode for keeping track of different aspects of
/// an animation.
struct PhysiqueShapeSceneNodeMorphAnimationEntry
{
   /// Object with the channel controllers.
   MorphAnimationSceneNode* animationNode;
   /// Controls the timing of the animation.
   AnimationState animationState;
   /// Value used when prioritizing the influence of different animations.
   float priority;
   /// True if this animation should be blended additively.
   bool additive;
   /// Scratch space for the weight for a particular combination of morph
   /// channel and morph animation.
   float effectiveWeight;
   /// Weights for combinations of morph channel and morph animation.
   Table<float> channelWeights;
};

//=============================================================================

/// Specifies the degree to which a given bone influences a given vertex.
struct PhysiqueBoneVertexInfluence
{
   /// Index of the vertex affected by the bone.
   int vertexIndex;
   /// Degree of influence of the bone's transform upon the given vertex.
   float weight;
};

//=============================================================================

/// Specifies how a given bone affects vertices.
struct PhysiqueBoneInfluence
{
   /// Number of entries in 'vertexInfluences'.
   size_t numberOfVertices;
   /// List of vertices influenced by this bone and the degree to which they're influenced.
   PhysiqueBoneVertexInfluence* vertexInfluences;

   PhysiqueBoneInfluence(size_t _numberOfVertices)
   {
      numberOfVertices = _numberOfVertices;
      if(numberOfVertices)
         vertexInfluences = frog_new PhysiqueBoneVertexInfluence[numberOfVertices];
      else
         vertexInfluences = NULL;
   }
};

//=============================================================================

/// Part of a physique mesh to be drawn with a single native draw call.  This
/// includes vertex data, index data, and a description of the vertex
/// attributes.
struct PhysiqueShapeSubmeshCommon
{
   PhysiqueShapeSubmeshCommon(SceneLoadData* input, PhysiqueShapeData* physiqueShapeData);
   virtual ~PhysiqueShapeSubmeshCommon();

   /// Number of bones associated with this mesh.
   size_t numberOfBones;
   /// Number of vertices in the collection.
   uint32 numberOfVertices;
   /// Size of a singe vertex in bytes.
   size_t staticVertexSize;
   /// Submaterial number for this set of faces.
   unsigned char materialId;
   /// Vertex buffer for data that is not animated.
   void* staticVertexData;
   /// Number of faces to draw for this submesh.
   uint32 numberOfFaces;
   /// Whether the indices are uint16s, uint32s, etc.
   IndexDataType indexDataType;
   /// Index buffer.
   void* indexData;
   /// Whether the indices are uint16s, uint32s, etc.
   IndexDataType indexDataWithAdjacencyType;
   /// Index buffer for triangles with adjacency.
   void* indexDataWithAdjacency;
   /// Descriptions of the individual attributes for the map channels.
   Table<VertexAttribute> mapChannelVertexAttributes;
   /// Stores animation-independent data for individual mesh morph channels.
   Table<SubmeshMorphChannel> submeshMorphChannels;
   /// Specifies how specific bones affect specific vertices.
   PhysiqueBoneInfluence** boneInfluences;
   /// Mapping of submesh vertex indices to shared normals.
   uint32* sharedNormalIndices;
   /// Positions of the vertices before any transforms are applied.
   Point3F* vertexRestPositions;
   /// Positions of the vertices after morph targets have been applied.
   Point3F* vertexPositionsAfterMorphing;
   /// Positions of the vertices to be sent to the hardware.
   Point3F* finalVertexPositions;
   /// Normals of the vertices to be sent to the hardware.
   Point3F* finalVertexNormals;
};

//=============================================================================

struct PhysiqueShapeData
{
   /// Array of bones used to transform vertices.  The nodes exist as parts
   /// of a tree of bones, but this array lists them flat.
   SceneNode** boneList;
   size_t numberOfBones;

   /// Inverse of the transforms of the associated bones when it's in its rest pose.
   /// This is in the object space of the Physique object.
   Matrix43* inverseBoneRestPoseTransforms;

   /// Scratch space for transform from the rest positions of the bones to
   /// their current positions.
   Matrix43* boneChangeTransforms;

   /// Buffer of normals to be shared across submeshes to account for
   /// smoothing groups.
   uint32 numberOfSharedNormals;
   Point3F* sharedNormals;
   
   uint32 numberOfSubmeshes;
   PhysiqueShapeSubmesh** submeshes;

   Circle3 boundingSphere;
   /// Rough axis-aligned bounding box in the local space of the mesh.
   Box3F boundingBox;

   /// Ordered list of morph channels.
   Table<MorphChannel> morphChannels;

   PhysiqueShapeData(SceneLoadData* input);
   ~PhysiqueShapeData();
};

//=============================================================================

/// This shape is a type of dynamic mesh to be drawn by RenderSceneNode.  The
/// mesh is recomputed when necessary based on bone transforms and morph
/// targets, which makes it useful for characters.  In most other ways, it's
/// similar to MeshShapeSceneNodeCommon.  It only contains the code common to
/// all platforms, so it can't be used directly.  Use PhysiqueShapeSceneNode
/// instead.
class PhysiqueShapeSceneNodeCommon : public ShapeSceneNode
{
public:
   PhysiqueShapeSceneNodeCommon();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   // Objects further away from teh camera than the given update distance are not rephysiqued
   void UpdateDistanceSet(float a) { newUpdateDistance = a; }

   virtual void UpdateEarly(RenderData* renderData, bool parentTransformDirty);
   /// For now, this is not necessarily accurate for Physique meshes.
   virtual const Circle3& BoundingSphereGet(RenderData* renderData) const;
   /// For now, this is not necessarily accurate for Physique meshes.
   virtual const Box3F& BoundingBoxGet(RenderData* renderData) const;
   virtual void Intersect(RenderData* renderData, IntersectResponder* responder, const Ray3& ray, IntersectRecord& intersectRecord);
   virtual Point3F NormalGet(RenderData* renderData, const IntersectRecord& record);

   /// Return the number of morph channels in this mesh.
   int MorphChannelCountGet();
   /// Return the name of the given morph channel.  Return NULL if unsuccessful.
   const char* MorphChannelNameGet(int morphTargetIndex);
   /// Return the index of a given morph channel.  Return -1 if unsuccessful.
   int MorphChannelIndexGet(const char* morphChannelName);

   int MorphAnimationCountGet() { return morphAnimations.SizeGet(); }
   /// Add the given morph animation and initialize its AnimationState to the
   /// given weight.
   void MorphAnimationAdd(MorphAnimationSceneNode* animationNode, float animationWeight);
   /// Add a new animation based on a subset of the timeline
   /// of the given animation.  'startTime' and 'duration' are in seconds.
   /// Initialize the AnimationState to the given weight.
   void MorphAnimationAdd(int originalAnimationIndex, float startTime,
      float duration, float animationWeight);
   /// Return the specified morph animation.
   MorphAnimationSceneNode* MorphAnimationGet(int animationIndex);
   
   /// Return the AnimationState for the specified morph animation.
   AnimationState* MorphAnimationStateGet(int animationIndex);

   /// Set the value used for the given animation when prioritizing the
   /// influence of different animations.
   void MorphAnimationPrioritySet(int animationIndex, float priority);
   /// Return the value used for the given animation when prioritizing the
   /// influence of different animations.  This is 0.0f by default.
   float MorphAnimationPriorityGet(int animationIndex);

   /// Set whether additive blending should be used when determining the
   /// influence of the given animation.
   void MorphAnimationAdditiveSet(int animationIndex, bool additive);
   /// Return true if additive blending should be used when determining the
   /// influence of the given animation.  This is 'false' by default.
   bool MorphAnimationAdditiveCheck(int animationIndex);

   /// Set the weight for a specific morph channel of a specific morph
   /// animation.  This will be multiplied by the weight for the morph
   /// animation itself.
   void MorphAnimationChannelWeightSet(int animationIndex, int channelIndex, float weight);
   /// See 'MorphAnimationChannelWeightSet'.
   float MorphAnimationChannelWeightGet(int animationIndex, int channelIndex);

   virtual bool FullyOpaqueCheck(RenderData* renderData, MaterialSceneNode* material, AnimationState* materialAnimationState);

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

   typedef ShapeSceneNode Inherited;
   
protected:
   /// Helper function to get the entry for a given morph animation.
   PhysiqueShapeSceneNodeMorphAnimationEntry* GetMorphAnimationEntry(int animationIndex);

   /// Update the positions and normals of the mesh vertices.
   void MeshUpdate(RenderData* renderData, SceneNode* renderNode);

   PhysiqueShapeData* mesh;
   mutable float updateDistance;
   float newUpdateDistance;
   /// Collection of animation-specific data.
   Table<PhysiqueShapeSceneNodeMorphAnimationEntry*> morphAnimations;
   /// True if the mesh has already been computed for the current frame.
   bool meshPrepeared;
};

//=============================================================================

/// This type of ShapeSceneNode delegates the shape to a squence of other
/// ShapeSceneNodes to be used over time.  It's a brute-force approach for
/// doing mesh animations in situations where using bones or morph targets
/// wouldn't be practical.  Beware that this can potentially use a lot of
/// memory.
class AnimatedShapeSceneNode : public ShapeSceneNode
{
public:
   AnimatedShapeSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   virtual void Render(RenderData* renderData, SceneNode* renderNode,
      MaterialSceneNode* material, AnimationState* materialAnimationState);
   virtual const Circle3& BoundingSphereGet(RenderData* renderData) const;
   virtual const Box3F& BoundingBoxGet(RenderData* renderData) const;
   virtual void Intersect(RenderData* renderData, IntersectResponder* responder, const Ray3& ray, IntersectRecord& intersectRecord);
   virtual Point3F NormalGet(RenderData* renderData, const IntersectRecord& record);
   
   AnimationState* AnimationStateGet() { return &animationState; }

   virtual bool FullyOpaqueCheck(RenderData* renderData, MaterialSceneNode* material, AnimationState* materialAnimationState);
   
   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) AnimatedShapeSceneNode(); }

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

   /// Called when a material is assigned to a shape.
   virtual void OnMaterialSet(MaterialSceneNode* material);

private:
   virtual void Render(RenderData*) { }

   int timeScale;

   AnimationState animationState;

   size_t numberOfFrames;
   ShapeSceneNode** frames;

   typedef ShapeSceneNode Inherited;
};

//=============================================================================

/// This shape draws a regular Frog Sprite from a RenderSceneNode. It only
/// contains the code common to all platforms, so it can't be used directly.
/// Use SpriteShapeSceneNode instead.
class SpriteShapeSceneNodeCommon : public ShapeSceneNode
{
public:
   void Init(const char* resourceFilename, const char* animationName);
   virtual void Deinit();
   
   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   virtual const Circle3& BoundingSphereGet(RenderData* renderData) const;
   virtual const Box3F& BoundingBoxGet(RenderData* renderData) const;
   virtual void Intersect(RenderData*, IntersectResponder*, const Ray3&, IntersectRecord&) { }
   virtual Point3F NormalGet(RenderData*, const IntersectRecord&) { return Point3F::Create(0.0f, -1.0f, 0.0f); }

   // For now, assume it's not fully opaque.
   virtual bool FullyOpaqueCheck(RenderData*, MaterialSceneNode*, AnimationState*) { return false; }

   /// Set the current number of milliseconds into the sprite animation.
   void TimeSet(int _time) { sprite.TimeSet(_time); }
   /// Set how much to rotate the sprite about its origin.  Positive rotations are counter-clockwise.
   /// Angles should be given in degrees.
   void RotationSet(float _rotation) { sprite.RotationSet(_rotation); }
   /// Set the scale of the sprite such that the current animation would have
   /// the given dimensions in world space.
   void SizeSet(const Point2F& size);
   /// Set the color to apply to all vertices when drawing.
   /// By manipulating the 'alpha' component, you can control the opacity of the sprite.
   void ColorSet(const ColorRGBA8& _color) { sprite.ColorSet(_color); }
   /// Set '_additiveBlending' to [0, 1] depending on whether you want normal
   /// alpha blending, additive blending, or something in between.
   void AdditiveBlendingSet(float _additiveBlending) { sprite.AdditiveBlendingSet(_additiveBlending); }
   /// Returns the sprite object owned by this node.
   Sprite* SpriteGet() { return &sprite; }

   /// Return the longest duration for one of this node's controllers.
   virtual float ControllerTimeMaxGet() const;

   typedef ShapeSceneNode Inherited;

protected:
   /// Based on the current animation and scale, compute the bounding box and
   /// sphere.
   void BoundsRefresh();

   Sprite sprite;
   /// Axis-aligned bounding box in the local space of the mesh.
   Box3F boundingBox;
   Circle3 boundingSphere;
};

//=============================================================================

/// This is a shape that draws text from a RenderSceneNode.  It only contains
/// the code common to all platforms, so it can't be used directly.  Use
/// TextShapeSceneNode instead.
class TextShapeSceneNodeCommon : public ShapeSceneNode
{
public:
   /// Initialize the node to use the given font.  Set '_shouldUnloadFont' to
   /// true if the font should automatically be unloaded when it is no longer
   /// needed.
   void Init(Font* _font, bool _shouldUnloadFont = false);
   void Init(const char* fontFilename);
   virtual void Deinit();
   
   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   virtual const Circle3& BoundingSphereGet(RenderData* renderData) const;
   virtual const Box3F& BoundingBoxGet(RenderData* renderData) const;
   virtual void Intersect(RenderData*, IntersectResponder*, const Ray3&, IntersectRecord&) { }
   virtual Point3F NormalGet(RenderData*, const IntersectRecord&) { return Point3F::Create(0.0f, -1.0f, 0.0f); }

   // For now, assume it's not fully opaque.
   virtual bool FullyOpaqueCheck(RenderData*, MaterialSceneNode*, AnimationState*) { return false; }

   /// Set the color to apply to all vertices when drawing.
   /// By manipulating the 'alpha' component, you can control the opacity of the sprite.
   void ColorSet(const ColorRGBA8& _color) { textColor = _color; }
   /// Set '_additiveBlending' to [0, 1] depending on whether you want normal
   /// alpha blending, additive blending, or something in between.
   void AdditiveBlendingSet(float _additiveBlending) { additiveBlending = _additiveBlending; }

   /// Sets the font to use.  Set '_shouldUnloadFont' to true of this font
   /// should be unloaded automatically when it is no longer needed.
   void FontSet(Font* _font, bool _shouldUnloadFont = false);
   /// Switch to using the given font.
   void FontSet(const char* fontFilename);
   /// Returns the font.
   Font* FontGet() { return font; }

   /// Set the text alignment.
   virtual void TextAlignmentSet(int _textAlignment) { textAlignment = _textAlignment; BoundsRefresh(); }
   /// Set the box to which the text should be aligned and clipped.
   virtual void TextBoundsSet(Box2F _textBounds) { textBounds = _textBounds; BoundsRefresh(); }

    /// Return the text to be displayed by this node.
   virtual const char* TextGet() { return text; }
   /// Set the text to be displayed by this node.
   virtual void TextSet(const char* _text);

   /// Return the key for theText for the string to be displayed by this
   /// node.  Return NULL if a text key is not in use.
   virtual const char* TextKeyGet() { return textKey; }
   /// Set the key for theText for the string to be displayed by this
   /// node.  This will replace any existing text, including when called with
   /// NULL.  This method will call TextRefresh() internally if appropriate.
   virtual void TextKeySet(const char* _textKey);

   /// Reload the text for the widget.
   /// Call this after a language change.
   virtual void TextRefresh();

   /// Set the scale of the text such that the current text would have
   /// the given dimensions in world space. The width is dependant on the height 
   /// to maintain aspect ratio.
   void HeightSet(float _height);

   /// Set whether the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   virtual void TextAscentDescentMaxUseSet(bool _textAscentDescentMaxUse) { textAscentDescentMaxUse = _textAscentDescentMaxUse; }

protected:
   /// Helper function for initialization.
   void InitHelper(Font* font, bool _shouldUnloadFont);
   /// Used to display the given text.
   void TextSetHelper(const char* _text);
   /// Based on the current settings, compute the bounding volumes and update
   /// the final scale to apply when drawing.
   void BoundsRefresh();
   /// Update the bounds of the current text in the current font without
   /// scaling or alignment applied.
   void TextOpaqueBoundsRefresh();

   /// The text to actually display.
   char* text;
   /// If defined, this will be passed to theText to get the string to display.
   char* textKey;
   /// Bounds of the current text in the current font without scaling or
   /// alignment applied.
   Box2F textOpaqueBounds;
   /// The font used to draw the text.
   Font* font;
   /// True if 'font' should be unloaded when it's no longer needed.
   bool shouldUnloadFont;
   /// Target height of the string in world space.
   float height;
   /// Axis-aligned bounding box in the local space of the mesh.
   Box3F boundingBox;
   /// Bounding sphere of the text in the local space of the mesh.
   Circle3 boundingSphere;
   /// The color to draw the text.
   ColorRGBA8 textColor;
    /// Combination of flags to use for alignment of the text.
   int textAlignment;
   /// Additive blending to apply when drawing.
   float additiveBlending;
   /// Bounds in which to draw the text
   Box2F textBounds;
   /// Scale at which to draw the font
   Point2F scale;
   /// True if the maximum ascent and descent of the font should be used
   /// when aligning text vertically, rather than using the height of the string itself.
   bool textAscentDescentMaxUse;
};

//=============================================================================

struct BezierPathData;

/// This object provides a 3D Bezier spline.
class BezierPathSceneNode : public PathSceneNode
{
public:
   BezierPathSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void PositionGet(RenderData* renderData, Point3F& position, float percent);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) BezierPathSceneNode(); }

private:
   size_t numberOfKeys;
   BezierPathData* keys;
};

//=============================================================================

/// This node represents a bone in a skeleton.  It includes features for
/// blending multiple transform animations.
class BoneSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   /// Set the weight for this bone for the given skeletal animation.
   void BoneAnimationWeightSet(int animationIndex, float weight);
   /// Return the weight for this bone for the given skeletal animation.
   float BoneAnimationWeightGet(int animationIndex);
   /// Set the weight for this bone and all descendant bones of the skeleton
   /// for the given skeletal animation.
   void BoneAnimationWeightSetRecursively(int animationIndex, float weight);
   
   /// Recursively add animation data from the given skeleton.  Initialize the
   /// weights of the individual bones to 'boneWeight'. 
   void BoneAnimationAddRecursively(BoneSceneNode* bone, float boneWeight);

   /// Recursively set up a new animation based on a subset of the timeline
   /// of the given animation.  'startTime' and 'duration' are in seconds.
   /// Weights of individual bones will be set to 'boneWeight'.
   void BoneAnimationAddRecursively(int originalAnimationIndex,
      float startTime, float duration, float boneWeight);
   
   /// Add the given transform controller to the collection of animations for
   /// this bone, and initialize the bone weight in that animation to the given
   /// value.  
   void BoneAnimationAdd(TransformControllerSceneNode* animationTransformController, float boneWeight);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) BoneSceneNode(); }
   
   typedef TransformSceneNode<GroupSceneNode> Inherited; 
         
protected:
   /// Helper function for determining the duration of a given animation.
   float AnimationControllerTimeMaxGet(int animationIndex);

   /// Weights associated with individual animations for this bone.
   Table<float> animationBoneWeights;
};

//==============================================================================

/// Represents the root bone of a skeleton as well as properties that apply to
/// the skeleton as a whole.  It includes features for blending transform
/// animations.
class SkeletonSceneNode : public BoneSceneNode
{
public:
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);
   
   int SkeletalAnimationCountGet() { return skeletalAnimations.SizeGet(); }

   /// Return an AnimationState that applies to the entire skeleton.
   AnimationState* SkeletalAnimationStateGet(int animationIndex);
   
   /// Set the value used for the given animation when prioritizing the
   /// influence of different animations.
   void SkeletalAnimationPrioritySet(int animationIndex, float priority);
   /// Return the value used for the given animation when prioritizing the
   /// influence of different animations.  This is 0.0f by default.
   float SkeletalAnimationPriorityGet(int animationIndex);

   /// Set whether additive blending should be used when determining the
   /// influence of the given animation.
   void SkeletalAnimationAdditiveSet(int animationIndex, bool additive);
   /// Return true if additive blending should be used when determining the
   /// influence of the given animation.  This is 'false' by default.
   bool SkeletalAnimationAdditiveCheck(int animationIndex);

   /// See 'SkeletalAnimationRotationOnlyCheck'.
   void SkeletalAnimationRotationOnlySet(bool _rotationOnly) { rotationOnly = _rotationOnly; }
   /// Return true if the transforms of non-root bones should only vary from
   /// the rest pose in terms of rotation.  If this returns true, this currently
   /// assumes there is no scaling in the rest pose.
   bool SkeletalAnimationRotationOnlyCheck() { return rotationOnly; }
   
   /// Recursively add animation data from the given skeleton.  Initialize the
   /// animation weight and the weights of the individual bones to the
   /// specified values. 
   void BoneAnimationAddRecursively(BoneSceneNode* bone, float boneWeight, float animationWeight);

   /// Recursively set up a new animation based on a subset of the timeline
   /// of the given animation.  'startTime' and 'duration' are in seconds.
   /// Weights of individual bones will be set to 'boneWeight'.  The weight for
   /// the animation as a whole will be set to 'animationWeight'.
   void BoneAnimationAddRecursively(int originalAnimationIndex, float startTime,
      float duration, float boneWeight, float animationWeight);
   
   /// Add the given transform controller to the collection of animations for
   /// this bone, and initialize the weight the bone in that animation to the
   /// given value.  This will also add an AnimationState and initialize the
   /// weight to 'animationWeight'.  
   void BoneAnimationAdd(TransformControllerSceneNode* animationTransformController, float boneWeight, float animationWeight);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) SkeletonSceneNode(); }
   
   typedef BoneSceneNode Inherited;

protected:
   /// Helper function that adds a new AnimationState objects as needed based
   /// on animations in the underlying BoneTransformControllerSceneNode.  
   void BoneAnimationStatesAdd(float animationWeight);
   /// Helper function for finding an entry for a given animation.
   SkeletonSceneNodeAnimationEntry* SkeletalAnimationGet(int animationIndex);
   
   /// See 'SkeletalAnimationRotationOnlyCheck'.
   bool rotationOnly;
   /// Controls the properties of the animations that affect the skeleton as a
   /// whole.
   Table<SkeletonSceneNodeAnimationEntry*> skeletalAnimations;
};

//==============================================================================

/// BoneTransformControllerSceneNode is a transform controller which allows
/// the transform to be determined by combinations of other transform
/// controllers for various animations.  It requests the animation states
/// of individual animations from the parent bone and root bone.  
class BoneTransformControllerSceneNode : public TransformControllerSceneNode
{
public:
   BoneTransformControllerSceneNode();
   
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual void UpdateEarly(RenderData*, bool parentTransformDirty);

   virtual void TransformApply(Matrix43& transform, RenderData*);
   virtual void TransformInverseApply(Matrix43& transform, RenderData*);
   virtual float ControllerTimeMaxGet() const;
   
   /// Add a transform controller for another animation.
   void AnimationTransformControllerAdd(TransformControllerSceneNode* animationTransformController);
   /// Return the transform controller for the given animation.
   TransformControllerSceneNode* AnimationTransformControllerGet(int animationIndex);

   /// Return the number of animations.
   int AnimationCountGet() { return animationTransformControllers.SizeGet(); }

   /// Tell the controller the bone to which it corresponds.
   void BoneSet(BoneSceneNode* _bone) { bone = _bone; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) BoneTransformControllerSceneNode(); }
   
   typedef TransformControllerSceneNode Inherited; 
   
protected:
   /// Set 'localTransform' to the local transform of this controller.
   void TransformLocalGetHelper(Matrix43& localTransform, RenderData* renderData);

   /// Return the root bone of the skeleton.
   SkeletonSceneNode* SkeletonRootGet();
      
   /// Controllers for the transform of this bone for each animation.
   Table<TransformControllerSceneNode*> animationTransformControllers;
   /// Scratch space used when computing weights.
   Table<float> animationEffectiveWeights;
   
   /// Bone corresponding to this transform controller.
   BoneSceneNode* bone;
   /// Cached root of the skeleton.
   SkeletonSceneNode* skeleton;
   
   /// Translation component of the bone's local transform.
   Point3F localTranslation;
   /// Rotation component of the bone's local transform.  
   Quaternion localRotation;
   /// Default translation component of the bone's local transform.
   Point3F localTranslationDefault;
   /// Default rotation component of the bone's local transform.
   Quaternion localRotationDefault;
};

//=============================================================================

/// This material allows a single shape, like a MeshShapeSceneNode or
/// PhysiqueShapeSceneNode, to effectively use different materials for
/// different parts of the mesh.  Each submesh can request a specific
/// submaterial by its ID number within this material.
class MultiSubMaterialSceneNode : public MaterialSceneNode
{
public:
   MultiSubMaterialSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual MaterialSceneNode* MaterialGet(RenderData* renderData, int subMaterialId, AnimationState* materialAnimationState);

   virtual void DebugPrint(int depth);
   
   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) MultiSubMaterialSceneNode(); }

   virtual bool FullyOpaqueCheck(RenderData* renderData, int subMaterialId, AnimationState* materialAnimationState);

   virtual float ControllerTimeMaxGet() const;

private:
   size_t numberOfSubMaterials;
   MaterialSceneNode** subMaterials;
};

//=============================================================================

/// This material uses a brute-force approach to material animation.  It
/// contains a series of other materials to be used over time.  This is useful
/// for situations where the material needs to change in a major way over time,
/// like using different textures on each frame.
class AnimatedMaterialSceneNode : public MaterialSceneNode
{
public:
   AnimatedMaterialSceneNode();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual MaterialSceneNode* MaterialGet(RenderData* renderData, int subMaterialId, AnimationState* materialAnimationState);

   virtual void DebugPrint(int depth);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) AnimatedMaterialSceneNode(); }

   virtual bool FullyOpaqueCheck(RenderData* renderData, int subMaterialId, AnimationState* materialAnimationState);

   virtual float ControllerTimeMaxGet() const;

private:
   float timeScaleFactor;
   float timeOffset;

   size_t numberOfSubMaterials;
   MaterialSceneNode** subMaterials;

   typedef MaterialSceneNode Inherited;
};

//=============================================================================

/// This class helps a material apply a texture.  It includes the texture
/// itself, as well as information like which texture coordinate channel of the
/// mesh to use and how to wrap the UV coordinates.  It only contains the code
/// common to all platforms, so it can't be used directly.  Use
/// TextureSceneNode instead.
class TextureSceneNodeCommon : public SceneNode
{
public:
   TextureSceneNodeCommon();

   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
      
   virtual const char* NameGet() const { return name; }

   /// Return the underlying Texture object.
   Texture* TextureGet() const { return texture; }

   /// Return the index of the mesh map channel for this texture's coordinates.
   int TexCoordMapChannelGet() { return texCoordMapChannel; }

   /// Return the texture UV offset to use.
   void UVOffsetGet(Point2F& uvOffset, RenderData* renderData);
   /// Return true if the UV offset is always zero.
   bool UVOffsetConstantZeroCheck();

   /// Return the texture UV scale to use.
   void UVScaleGet(Point2F& uvScale, RenderData* renderData);
   /// Return true if the UV scale is always 1.
   bool UVScaleConstantOneCheck();

   virtual float ControllerTimeMaxGet() const;
   
protected: 
   /// Object to represent the texture itself.
   Texture* texture;
   /// Name of the node.
   const char* name;
   /// Index of the mesh map channel for this texture's coordinates.
   int texCoordMapChannel;
   /// How to wrap the texture in the 'U' axis.
   TextureWrapMode uWrapMode;
   /// How to wrap the texture in the 'V' axis.
   TextureWrapMode vWrapMode;
   /// Whether to generate mipmaps for the texture.
   bool generateMipmaps;
   /// How to filter the texture.
   TextureFilterMode filterMode;
   /// Offset of the 'U' texture coordinate over time.
   ValueControllerSceneNode* uOffsetControllerNode;
   /// Offset of the 'V' texture coordinate over time.
   ValueControllerSceneNode* vOffsetControllerNode;
   /// Scale applied to the 'U' texture coordinate over time.
   ValueControllerSceneNode* uScaleControllerNode;
   /// Scale applied to the 'V' texture coordinate over time.
   ValueControllerSceneNode* vScaleControllerNode;
};

//=============================================================================

/// This is a general-purpose material.  It provides common features, like
/// diffuse textures, light maps, and specularity.  It only contains the code
/// common to all platforms, so it can't be used directly.  Use
/// StandardMaterialSceneNode instead.
class StandardMaterialSceneNodeCommon : public MaterialSceneNode
{
public:
   StandardMaterialSceneNodeCommon();
   
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual void DebugPrint(int depth);
   virtual bool FullyOpaqueCheck(RenderData* /*renderData*/, int /*subMaterialId*/,
      AnimationState* /*materialAnimationState*/) { return fullyOpaque; }

   virtual float ControllerTimeMaxGet() const;

protected:
   /// Diffuse material color to use if no diffuse texture was specified.
   ColorRGBA8 diffuseColor;
   /// Texture to be sampled for the diffuse color.
   TextureSceneNode* diffuseTextureNode;
   /// Specular material color.
   ColorRGBA8 specularColor;
   /// Coefficient applied to the specular reflection.
   float specularLevel;
   /// Controls the sharpness of specular highlights. [0, 1]
   float specularGlossiness;
   /// Color to act like a lightmap.
   ColorRGBA8 selfIlluminationColor;
   /// Texture to be sampled for the self illumination color.
   TextureSceneNode* selfIlluminationTextureNode;
   /// Opacity of the material over time.
   ValueControllerSceneNode* opacityControllerNode;
   /// True if a silhouette should be drawn.
   bool silhouette;
   /// Cached flag for whether to attempt to use a diffuse texture.
   bool useDiffuseTexture;
   /// Cached flag for whether to attempt to use a self illumination texture.
   bool useSelfIlluminationTexture;
   /// Cached flag for whether this material is fully opaque.
   bool fullyOpaque;
   /// Thickness to use when drawing silhouette edges.
   float silhouetteEdgeWidth;
   /// Cached flag for whether to perform lighting.
   bool applyLighting;
   /// Cached flag for whether to add specular highlights.
   bool applySpecularity;
   /// Cached flag for whether to apply a self illumination color.
   bool applySelfIlluminationColor;
   /// Cached flag for whether to apply UV offsets and scale for the diffuse texture.
   bool applyDiffuseTexCoordOffsetScale;
   /// Cached flag for whether to apply the modulative color, which includes
   /// opacity.
   bool applyModulativeColor;
   /// True if the both the front face and back face should be rendered.
   bool twoSided;
   /// True if face culling should be re-enabled when cleaning up after this
   /// pass.
   bool restoreFaceCullingEnabled;
   /// True if face culling should be set to bac faces when cleaning up after
   /// this pass.
   bool restoreBackfaceCulling;
};

//=============================================================================

/// Cameras can be animated and exported from 3ds Max.  This can be useful for
/// cinematics when used with CameraController_AttachToSceneNode.  The
/// coordinate system of an exported camera is different from most other nodes.
/// +x is forward, +y is up, and +z is back.
class CameraSceneNode : public TransformSceneNode<SceneNode>
{
public:  
   CameraSceneNode();
   
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();
   
   virtual const char*  NameGet() const { return name; }
   
   virtual const Matrix44& ProjectionMatrixGet(RenderData*) { return projectionMatrix; }
   
   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) CameraSceneNode(); }

private:
   const char* name;
   Matrix44 projectionMatrix;
   
   typedef TransformSceneNode<CameraSceneNode> Inherited;
};

//=============================================================================

/// This represents a dynamic light.
class LightSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   virtual void Init();
   virtual void Init(SceneLoadData* input);
   
   virtual void Intersect(RenderData*, const Ray3&, IntersectRecord&) { }
   virtual void DebugPrint(int depth);

   /// Return the type of the light.
   LightType LightTypeGet() { return lightType; }
   /// Set the type of the light.
   void LightTypeSet(LightType _lightType) { lightType = _lightType; }

   /// Return the combined color and intensity of the light.
   ColorRGB8 LightColorGet() { return color; }
   /// Set the combined color and intensity of the light.
   void LightColorSet(const ColorRGB8& _color) { color = _color; }

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) LightSceneNode(); }

private:
   typedef TransformSceneNode<GroupSceneNode> Inherited;

   /// Controls how the light is applied.
   LightType lightType;
   /// Combined color and intensity of the light.
   ColorRGB8 color;
   /// Helper for building a list of lights in the scene.
   LightSceneNode* nextLight;

   friend class Webfoot::SceneCommon;
};

//=============================================================================

/// Base class for projecting things onto surfaces.
class ProjectorSceneNode : public TransformSceneNode<GroupSceneNode>
{
public:
   void Init();

   /// Apply the projector to the given scene.
   virtual void ApplyToScene(SceneNode* rootSceneNode, RenderData* renderData) = 0;

   virtual void Intersect(RenderData*, const Ray3&, IntersectRecord&) { }

   /// Set the priority of the projector.  Projections with numerically higher
   /// priority will be drawn after those of lower priority.  The default
   /// priority is 0.0.
   void PrioritySet(float _priority) { priority = _priority; }
   /// Return the priority of the projector.
   float PriorityGet() { return priority; }

   static bool PriorityComparator(const ProjectorSceneNode* valueA, const ProjectorSceneNode* valueB)
   {
      return valueA->priority < valueB->priority;
   }

private:
   typedef TransformSceneNode<GroupSceneNode> Inherited;

   /// Helper for building a list of projectors in the scene.
   ProjectorSceneNode* nextProjector;
   /// Priority number for this projector.
   float priority;

   friend class Webfoot::SceneCommon;
};

//=============================================================================

/// Projects a texture onto a surface.  The transform of the projector uses the
/// same conventions as cameras.  It only contains the code common to all
/// platforms, so it can't be used directly.  Use TextureProjectorSceneNode
/// instead.
class TextureProjectorSceneNodeCommon : public ProjectorSceneNode
{
public:
   TextureProjectorSceneNodeCommon();
   
   void Init(const char* textureFilename);
   virtual void Deinit();

   /// Set the projection matrix for this projector.  Use OpenGL conventions
   /// as if this was for projecting into a framebuffer.
   void ProjectorMatrixSet(const Matrix44& _projectorMatrix);

   virtual void DebugPrint(int depth);

   typedef ProjectorSceneNode Inherited;

protected:
   /// Material used to draw the projection.
   TextureProjectorMaterialSceneNode* material;
   /// Matrix provided to this object for projecting the texutre.
   Matrix44 framebufferStyleProjectorMatrix;
   /// Matrix actually used to apply the texture.
   Matrix44 textureStyleProjectorMatrix;
};

//=============================================================================

/// Material used to project a texture onto a surface.  It only contains the
/// code common to all platforms, so it can't be used directly.  Use
/// TextureProjectorMaterialSceneNode instead.
class TextureProjectorMaterialSceneNodeCommon : public MaterialSceneNode
{
public:
   TextureProjectorMaterialSceneNodeCommon();

   void Init(const char* _textureFilename);
   virtual void Deinit();
   
   virtual void DebugPrint(int depth);

   virtual bool FullyOpaqueCheck(RenderData* /*renderData*/,
      int /*subMaterialId*/, AnimationState* /*materialAnimationState*/) { return true; }

protected:
   typedef MaterialSceneNode Inherited;

   /// Texture to be applied.
   Texture* texture;
   /// Filename for loading 'texture'.
   char textureFilename[FROG_PATH_MAX];
   /// Cached textureStyleProjectorMatrix * Inverse(worldTransform);
   Matrix44 cachedViewProjectorMatrix;

   friend class TextureProjectorSceneNode;
};

//=============================================================================

typedef TransformSceneNode<GroupSceneNode> SceneFileRootNode;

//=============================================================================

template<typename T> TransformSceneNode<T>::TransformSceneNode()
{
   transformController = NULL;
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::Init()
{
   T::Init();
   
   ConstantTransformControllerSceneNode* constantTransformController =
      frog_new ConstantTransformControllerSceneNode();
   constantTransformController->Init();
   transformController = constantTransformController;
   
   worldTransformCacheId         = -1;
   inverseWorldTransformCacheId  = -1;
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::Init(SceneLoadData* input)
{
   T::Init(input);
   
   transformController = dynamic_cast<TransformControllerSceneNode*>(SceneManager::Load(input));
   assert(transformController);
   transformAnimationState.DurationSet(transformController->ControllerTimeMaxGet());
   
   worldTransformCacheId         = -1;
   inverseWorldTransformCacheId  = -1;
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::Init(const TransformSceneNode& node, bool copyTransformController)
{
   T::Init(node);
   
   if(copyTransformController)
   {
      transformController = node.transformController;
      transformController->ReferenceAdd();
   }
   else
   {
      ConstantTransformControllerSceneNode* constantTransformController =
         frog_new ConstantTransformControllerSceneNode();
      constantTransformController->Init();
      transformController = constantTransformController;
   }

   transformAnimationState.DurationSet(transformController->ControllerTimeMaxGet());
   worldTransformCacheId         = -1;
   inverseWorldTransformCacheId  = -1;
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::Deinit()
{
   if(transformController)
   {
      transformController->ReferenceRemove();
      transformController = NULL;
   }

   T::Deinit();
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::UpdateEarly(RenderData* renderData, bool parentTransformDirty)
{
   // For now, don't try to establish whether the transform actually changed
   // since the last update. 
   parentTransformDirty = true;
   if(parentTransformDirty)
   {
      worldTransformCacheId = -1;
      inverseWorldTransformCacheId = -1;
   }
   transformAnimationState.Update(renderData->dtSeconds);
   T::UpdateEarly(renderData, parentTransformDirty);
   if(transformController)
      transformController->UpdateEarly(renderData, parentTransformDirty);
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::Update(RenderData* renderData)
{
   T::Update(renderData);
   if(transformController)
      transformController->Update(renderData);
}

//-----------------------------------------------------------------------------

template<typename T> const Matrix43& TransformSceneNode<T>::TransformWorldGet(RenderData* renderData)
{
   if(worldTransformCacheId != renderData->cacheId)
   {
      worldTransformCacheId = renderData->cacheId;
      float time = transformAnimationState.TimeGet();
      renderData->time = time;
      worldTransform = T::TransformWorldGet(renderData);
      renderData->time = time;
      transformController->TransformApply(worldTransform, renderData);
   }
   
   return worldTransform;
}

//-----------------------------------------------------------------------------

template<typename T> const Matrix43& TransformSceneNode<T>::TransformWorldInverseGet(RenderData* renderData)
{
   if(inverseWorldTransformCacheId != renderData->cacheId)
   {
      inverseWorldTransformCacheId = renderData->cacheId;
      float time = transformAnimationState.TimeGet();
      renderData->time = time;
      inverseWorldTransform = T::TransformWorldInverseGet(renderData);
      renderData->time = time;
      transformController->TransformInverseApply(inverseWorldTransform, renderData);
   }

   return inverseWorldTransform;
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::TransformControllerSet(TransformControllerSceneNode* newController)
{
   transformController->ReferenceRemove();
   transformController = newController;
   if(transformController)
   {
      transformController->ReferenceAdd();
      transformAnimationState.DurationSet(transformController->ControllerTimeMaxGet());
      transformAnimationState.TimeSet(0.0f);
   }
   TransformsInvalidate();
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::DebugPrint(int depth)
{
   const char* _name = T::NameGet();
   if(_name && *_name)
      DebugPrintf("%s, \"%s\"\n", T::DebugPrintLineGet(depth), _name);
   else
      DebugPrintf("%s\n", T::DebugPrintLineGet(depth));

   assert(transformController);

   DebugPrintf("%s{\n", T::DebugPrintSpacePrefixGet(depth));
   transformController->DebugPrint(depth+1);
   DebugPrintf("%s}\n", T::DebugPrintSpacePrefixGet(depth));

   T::DebugPrintChildren(depth);
}

//-----------------------------------------------------------------------------

template<typename T> void TransformSceneNode<T>::IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode, bool isConstant)
{
   if(!TransformControllerGet()->ConstantCheck()) isConstant = false;
   T::ChildIntersectionCacheGenerate(collisionNode, isConstant);
}

//-----------------------------------------------------------------------------

template<typename T> float TransformSceneNode<T>::ControllerTimeMaxGet() const
{
   float timeMax = T::ControllerTimeMaxGet();
   if(transformController)
      timeMax = std::max(timeMax, transformController->ControllerTimeMaxGet());
   return timeMax;
}

//=============================================================================

/// This helper object is used during the building of a collision heirarchy.
/// It serves as an entry for a single item, like a triangle, to be included
/// somewhere in the tree.
struct CollisionIntersectElementData
{
   /// Cached data for ray-triangle intersection tests with a given triangle.
   IntersectElement* element;
   /// Bounding box of the triangle in world space.
   Box3F boundingBox;
   /// Scene graph node (like a RenderSceneNode) which contains the triangle
   /// associated with this entry.
   IntersectProvider* provider;
   
   bool operator==(const CollisionIntersectElementData& a) const { return element == a.element && provider == a.provider; }
};

//=============================================================================

/// This node type facilitates optimized intersection tests for objects which
/// do not move.  See the explanation at the top of CollisionHierarchy.h.
class CollisionHierarchyRootNode : public GroupSceneNode
{
public:
   CollisionHierarchyRootNode();

   /// Initialize this object.  'aChildNode' should be the root of the tree of
   /// nodes with the static geometry for this hierarchy.
   void Init(SceneNode* aChildNode);
   virtual void Deinit();

   /// If you want a node with a dynamic transform to be included in this
   /// node's Intersect tests even though it isn't a child of this node, you
   /// can register it with this method.
   void DynamicNodeRegister(SceneNode* node);

   void IntersectElementRegister(IntersectElement* element, const Box3F& boundingBox, IntersectProvider* provider);

   void ChildAdd(SceneNode* node);
   virtual void ChildRemove(SceneNode* node);

   /// Generate the collision hierarchy.
   virtual void IntersectionCacheGenerate(CollisionHierarchyRootNode* collisionNode=NULL, bool isConstant=false);
   virtual void Intersect(RenderData* renderData, const Ray3& ray, IntersectRecord& intersectRecord);
   
private:
   typedef GroupSceneNode Inherited;

   /// Root node of the collision hierachy.
   IntersectNode* intersectNode;
   /// Collection of nodes to be checked in addition to the octree during an
   /// Intersect test.
   Table<SceneNode*> dynamicNodes;
   
   Table<CollisionIntersectElementData> intersectElementList;
};

//=============================================================================

template <typename IndexType>
void MeshShapeData::CollisionHierarchyDataGenerateHelper(CollisionHierarchyRootNode* collisionNode,
   const Matrix43& transform, IntersectProvider* provider,
   MeshIntersectCache** p, const MeshShapeSubmeshCommon* submesh)
{
   IndexType* indexBufferPosition = (IndexType*)submesh->indexData;
   
   for(size_t faceIndex = 0; faceIndex < submesh->numberOfFaces; faceIndex++, ++(*p))
   {
      // Get the indices of this face.
      IndexType index0, index1, index2;
      index0 = *indexBufferPosition; indexBufferPosition++;
      index1 = *indexBufferPosition; indexBufferPosition++;
      index2 = *indexBufferPosition; indexBufferPosition++;
      
      // Extract the vertex positions from the vertex buffer.  Use
      // memcpy in case the current platform can't handle misaligned
      // usage of floats.  This is assuming the position is the first
      // part of each vertex.
      Point3F p0, p1, p2;
      void* p0Source = (void*)((size_t)submesh->vertexData + (index0 * submesh->vertexSize));
      void* p1Source = (void*)((size_t)submesh->vertexData + (index1 * submesh->vertexSize));
      void* p2Source = (void*)((size_t)submesh->vertexData + (index2 * submesh->vertexSize));
      memcpy((void*)&p0, p0Source, sizeof(Point3F));
      memcpy((void*)&p1, p1Source, sizeof(Point3F));
      memcpy((void*)&p2, p2Source, sizeof(Point3F));
      
      Point3F v[3];
      v[0] = transform * p0;
      v[1] = transform * p1;
      v[2] = transform * p2;
      (*p)->Prepare(v);
      
      Box3F boundingBox;
      boundingBox.PositionSet(v[0]);
      boundingBox.SizeSet(0.0f, 0.0f, 0.0f);
      boundingBox |= v[1];
      boundingBox |= v[2];
      boundingBox.x -= (32.0f / 4096.0f);
      boundingBox.y -= (32.0f / 4096.0f);
      boundingBox.z -= (32.0f / 4096.0f);
      boundingBox.width += (64.0f / 4096.0f);
      boundingBox.height += (64.0f / 4096.0f);
      boundingBox.depth += (64.0f / 4096.0f);
      
      collisionNode->IntersectElementRegister(*p, boundingBox, provider);
   }
}
   
//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENENODESCOMMON_H__
