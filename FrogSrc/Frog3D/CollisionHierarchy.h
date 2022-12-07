#ifndef __FROG__FROG3D__COLLISIONHIERARCHY_H__
#define __FROG__FROG3D__COLLISIONHIERARCHY_H__

#include "FrogMemory.h"
#include "SceneNodes.h"

namespace Webfoot {

/// The CollisionHierarchy is a tree of container and leaf nodes.  Leafs have
/// associated SceneNodes with geometry that can be Intersected.  These
/// leaves store both the element (polygon) and the provider
/// (RenderSceneNode) of potential Intersections.  Container nodes just hold
/// leaf nodes and other container nodes.  Leaves and containers each have
/// their own bounding box and bounding sphere.  Calling
/// CollisionHierarchyRootNode::IntersectionCacheGenerate creates the
/// CollisionHierarchy.  First, it recursively builds a list of all the
/// things you can meaningfully Intersect.  Then it recursively goes through
/// that list building an octree.  Objects are sorted into the corners of the
/// bounding box based on their position.  If the nodes in a corner don't
/// overlap the edges, if there are enough nodes in that corner, and the
/// corner doesn't have most of the nodes in the box, it subdivides that
/// corner and recurses.  If combinations of corners have a small number of
/// children, they're merged to reduce the number of nodes.  Since billboards
/// and other animated nodes are dynamic, they are excluded from this tree and
/// placed in a separate list in the CollisionHierarchyRootNode.
///
/// When you create a CollisionHierarchyRootNode, you pass it the root of a
/// tree of SceneNodes, which becomes its only child.  When you perform most
/// recursive actions on the CollisionHierarchyRootNode, it defaults to
/// SceneNode behavior and passes the action to the child.  If you call
/// Intersect, however, it uses the CollisionHierarchy and the list of
/// dynamic nodes instead.  When you call Intersect on a container node, it
/// passes the Intersect call to any children whose bounding spheres
/// intersect the ray.  If you call Intersect on a leaf node, it passes the
/// call to all its SceneNodes.

//=============================================================================

/// This is the base class for the nodes of the collision hierarchy octree.
/// It can be either a container or a leaf.
struct IntersectNode
{
   /// Count has bottom bit set for leaf nodes, not set for containers
   size_t count;
   
   void Intersect(const Ray3& ray, IntersectRecord& intersectRecord) const;
   
   static IntersectNode* HierarchyCreate(Table<CollisionIntersectElementData> &intersectElementList);
   static IntersectNode* HierarchyCreate(Table<CollisionIntersectElementData> &intersectElementList, const Box3F& boundingBox);
   
   virtual ~IntersectNode() {}
};

//=============================================================================

/// This is a helper object with data for a given child of a
/// ContainerIntersectNode.
struct ContainerNodeData
{
   /// Bounding sphere of this child in world space.
   Circle3 boundingSphere;
   /// Bounding box of this child in world space.
   Box3F boundingBox;
   /// Child node.
   IntersectNode* node;
   
   void Intersect(const Ray3& ray, IntersectRecord& intersectRecord) const;
};

//=============================================================================

/// A node in a collision hierarchy octree which contains other nodes, but no
/// triangles.
struct ContainerIntersectNode : public IntersectNode
{
   ContainerIntersectNode(size_t count);
   virtual ~ContainerIntersectNode();
   
   void Intersect(const Ray3& ray, IntersectRecord& intersectRecord) const;

   /// Array of entries for this node's child nodes.  The number of children
   /// is stored in the inherited 'count' member, but you need to shift that
   /// number down by 1 bit to get the actual number of children.
   ContainerNodeData* data;
};

//=============================================================================

/// This is a helper object with data for a given triangle associated with a
/// LeafIntersectNode.
struct LeafNodeData
{
   /// Cached data for ray-triangle intersection tests with a given triangle.
   IntersectElement* element;
   /// Scene graph node (like a RenderSceneNode) which contains the triangle
   /// associated with this entry.
   IntersectProvider* provider;
   
   void Intersect(const Ray3& ray, IntersectRecord& intersectRecord) const
   { 
      provider->Intersect(ray, intersectRecord, element); 
   }
};

//=============================================================================

/// A node in a collision hierarchy octree which contains data on triangles.
struct LeafIntersectNode : public IntersectNode
{
   LeafIntersectNode(const Table<CollisionIntersectElementData> &intersectElementList);
   virtual ~LeafIntersectNode();
   
   void Intersect(const Ray3& ray, IntersectRecord& intersectRecord) const;
   
   /// Array of entries for the triangles associated with this node.  The
   /// number of entries is stored in the inherited 'count' member, but you
   /// need to shift that number down by 1 bit to get the actual number of
   /// children.
   LeafNodeData* data;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__COLLISIONHIERARCHY_H__
