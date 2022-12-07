#ifndef __FROG__DUCK__SCENENODE_H__
#define __FROG__DUCK__SCENENODE_H__

#include "FrogMemory.h"
#include "FileManager.h"
#include "Matrix43.h"
#include "Point3.h"
#include "Quaternion.h"
#include "Table.h"
#include "Utility.h"

namespace Webfoot {

class Font;
class JSONValue;

namespace Duck {

class Drawable;
class Scene;
class SceneInstance;
class SceneNodeRayIntersectResult;

/// Name of a scene node.
#define DUCK_SCENE_NODE_NAME_KEY "Name"
/// Type of a scene node.
#define DUCK_SCENE_NODE_TYPE_KEY "Type"
/// False if this node should be hidden, independently of its ancestors
/// and descendants.
#define DUCK_SCENE_NODE_VISIBLE_SPECIFIC_KEY "VisibleSpecific"
/// False if this node has been set explicitly so that it and its
/// descendants should be hidden as a group.
#define DUCK_SCENE_NODE_VISIBLE_HIERARCHICAL_KEY "VisibleHierarchical"
/// False if this node should be excluded for collision detection,
/// independently of its ancestors and descendants.
#define DUCK_SCENE_NODE_COLLIDABLE_SPECIFIC_KEY "CollidableSpecific"
/// False if this node has been set explicitly so that it and its descendants
/// should be excluded for collision detection as a group.
#define DUCK_SCENE_NODE_COLLIDABLE_HIERARCHICAL_KEY "CollidableHierarchical"
/// Position of a scene node relative to its parent.
#define DUCK_SCENE_NODE_POSITION_OFFSET_KEY "PositionOffset"
/// Rotation of a scene node relative to its parent.
#define DUCK_SCENE_NODE_ROTATION_KEY "Rotation"
/// Scale of a scene node relative to its parent.
#define DUCK_SCENE_NODE_SCALE_KEY "Scale"
/// Depth-sorted nodes of numerically higher priority are drawn after
/// depth-sorted nodes of lower priority.  Depth-sorted nodes of the same
/// priority are sorted by depth.  The default priority is 0.
#define DUCK_SCENE_NODE_DEPTH_SORT_PRIORITY_KEY "DepthSortPriority"
/// Children of a scene node.
#define DUCK_SCENE_NODE_CHILDREN_KEY "Children"
/// How the object should be automatically rotated, if at all.
#define DUCK_SCENE_NODE_BILLBOARD_MODE_KEY "BillboardMode"
/// Scale which should be applied to bounding volumes, like bounding spheres,
/// for culling purposes.
#define DUCK_SCENE_NODE_BOUNDING_VOLUME_SCALE_KEY "BoundingVolumeScale"
/// True if this node's children form an LOD group.
#define DUCK_SCENE_NODE_LOD_GROUP_KEY "LODGroup"
/// If this value is greater than 0 and this node is an LOD group, don't draw
/// any of this node's children if the distance from the camera is greater
/// than this amount.
#define DUCK_SCENE_NODE_LOD_GROUP_DRAW_DISTANCE_MAX_KEY "LODGroupDrawDistanceMax"
/// Don't draw this version if the distance to the camera is less than this
/// amount.  Try to draw a higher-quality version instead.
#define DUCK_SCENE_NODE_LOD_LEVEL_DRAW_DISTANCE_MIN_KEY "LODLevelDrawDistanceMin"
/// Name of the sprite resource file to use for this scene node and its
/// decendants.
#define DUCK_SCENE_NODE_SPRITE_RESOURCE_FILENAME_KEY "SpriteResourceFilename"

//==============================================================================

/// Base class for nodes in a Duck scene graph.
class SceneNode
{
public:
   /// How the object should be automatically rotated, if at all.
   enum BillboardMode
   {
      BILLBOARD_MODE_INVALID = -1,
      /// The object behaves as usual.
      BILLBOARD_MODE_DISABLED,
      /// Align the object so that its local +y direction pointing toward the 
      /// viewer and perpendicular to the projection plane.
      BILLBOARD_MODE_BILLBOARD,
      /// Rotate the object about its local z axis trying to make its local +y
      /// direction point toward the user and perpendicular to the projection
      /// plane.
      BILLBOARD_MODE_UPRIGHT,
      BILLBOARD_MODE_COUNT
   };

   /// Types of drawables that can be gathered.
   enum DrawablesGatherMode
   {
      /// Gather only drawables that do not require alpha blending.
      DRAWABLES_GATHER_MODE_OPAQUE,
      /// Gather only drawables that may require alpha blending.
      DRAWABLES_GATHER_MODE_TRANSPARENT,
      /// Gather only drawables that cast shadows.
      DRAWABLES_GATHER_MODE_SHADOW_CASTER
   };

   SceneNode();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of this node.
   const char* NameGet() { return name; }
   /// Set the name of this node.
   void NameSet(const char* _name) { StringDelete(name); name = StringClone(_name); }
   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   /// Return the parent of this node.
   SceneNode* ParentGet() { return parent; }
   /// Set the parent of this node.
   void ParentSet(SceneNode* _parent) { parent = _parent; }

   /// Return the SceneInstance to use for this node.  If a SceneInstance is not
   /// defined for the node, this will defer to the node's ancestors.  If there
   /// is no SceneInstance defined and no parent, NULL will be returned.
   SceneInstance* SceneInstanceGet();
   /// Set the SceneInstance to use.  Set to NULL to make the node use its
   /// parent's SceneInstance.  This can be called before Init.
   void SceneInstanceSet(SceneInstance* _sceneInstance) { sceneInstance = _sceneInstance; }

   /// Return true if the node should be displayed.  This includes the
   /// influence of whether this node has been explicitly hidden and whether
   /// one of its ancestors is set to hide descendants.
   bool VisibleEffectiveCheck() { return visibleSpecific && VisibleHierarchicalCheck(); }
   /// Set whether if this node should be hidden, independently of its ancestors
   /// and descendants.
   void VisibleSpecificSet(bool _visibleSpecific) { visibleSpecific = _visibleSpecific; }
   /// Return false if this node should be hidden, independently of its ancestors
   /// and descendants.
   bool VisibleSpecificCheck() { return visibleSpecific; }
   /// Set whether this node and its descendants should be hidden as a group.
   void VisibleHierarchicalSet(bool _visibleHierarchical) { visibleHierarchical = _visibleHierarchical; }
   /// Return false if this node has been set explicitly so that it and its
   /// descendants should be hidden as a group.
   bool VisibleHierarchicalExplicitCheck() { return visibleHierarchical; } 
   /// Return false if this node and its descendants should be hidden as a
   /// group, possibly as part of a larger group due to an ancestor.
   bool VisibleHierarchicalCheck() { return visibleHierarchical && (!parent || parent->VisibleHierarchicalCheck()); }

   /// Return true if the node should be included for collision detection.
   /// This includes the influence of whether this node has been explicitly
   /// excluded and whether one of its ancestors is set to exclude descendants.
   bool CollidableEffectiveCheck() { return collidableSpecific && CollidableHierarchicalCheck(); }
   /// Set whether if this node should be excluded for collision detection,
   /// independently of its ancestors and descendants.
   void CollidableSpecificSet(bool _collidableSpecific) { collidableSpecific = _collidableSpecific; }
   /// Return false if this node should be excluded for collision detection,
   /// independently of its ancestors and descendants.
   bool CollidableSpecificCheck() { return collidableSpecific; }
   /// Set whether this node and its descendants should be excluded for
   /// collision detection as a group.
   void CollidableHierarchicalSet(bool _collidableHierarchical) { collidableHierarchical = _collidableHierarchical; }
   /// Return false if this node has been set explicitly so that it and its
   /// descendants should be excluded for collision detection as a group.
   bool CollidableHierarchicalExplicitCheck() { return collidableHierarchical; } 
   /// Return false if this node and its descendants should be excluded for
   /// collision detection as a group, possibly as part of a larger group due
   /// to an ancestor.
   bool CollidableHierarchicalCheck() { return collidableHierarchical && (!parent || parent->CollidableHierarchicalCheck()); }

   /// Return the local position of the node.
   Point3F PositionRelativeGet() { return position; }
   /// Set the local position of the node.
   void PositionRelativeSet(const Point3F& _position) { position = _position; }

   /// Return the local rotation of the node.
   Quaternion RotationRelativeGet();
   /// Set the local rotation of the node.
   void RotationRelativeSet(const Quaternion& _rotation) { rotation = _rotation; }

   /// Return the local scale of the node.
   Point3F ScaleRelativeGet() { return scale; }
   /// Set the local scale of the node.
   void ScaleRelativeSet(const Point3F& _scale) { scale = _scale; }

   /// Return the local transform of the node.
   Matrix43 TransformRelativeGet();
   /// Return the world-space transform of the node.
   Matrix43 TransformAbsoluteGet();

   /// Return the inverse of the local transform of the node.
   Matrix43 TransformInverseRelativeGet();
   /// Return the inverse of the world-space transform of the node.
   Matrix43 TransformInverseAbsoluteGet();

   /// Return true if this node's local transform should be considered relative
   /// to its parent and false if the local transform should be treated as also
   /// the world transform.
   bool RelativeToParentCheck() { return relativeToParent; }
   /// See 'RelativeToParentCheck'.
   void RelativeToParentSet(bool _relativeToParent) { relativeToParent = _relativeToParent; }

   /// Add the node to the end of the list of children.
   void ChildAdd(SceneNode* child);

   /// Remove the given child from this node.  Return true if successful.
   bool ChildRemove(SceneNode* child);
   /// Remove the child at the end of the list.  Return true if successful.
   bool ChildLastRemove();

   /// Remove all the children.
   void ChildrenRemoveAll();

   /// Without adding the child to the garbage or implicitly setting its parent
   /// to NULL, remove the given child from the container.  Return true if
   /// successful.
   bool ChildDetach(SceneNode* child);

   /// Return a pointer to the first child with the given name.  Return NULL if
   /// the child is not found.
   SceneNode* ChildGet(const char* _name);
   /// Return a pointer to the child with the given index.  Return NULL if the
   /// index is out of bounds.
   SceneNode* ChildGet(int index);
   /// Return a pointer to the last child in the list.  Return NULL if the
   /// there are no children.
   SceneNode* ChildLastGet();

   /// Return the number of immediate children.
   int ChildCountGet() { return children.SizeGet(); }

   /// Reserve space for the given number of children.
   void ChildrenReserve(int childCount) { children.Reserve(childCount); }

   /// Load the given scene file and add its root to the end of the list of
   /// children.  Return a pointer to the root if successful.
   SceneNode* ChildCreate(const char* sceneFilename, FileManager* fileManager = theFiles);
   /// Create a SceneNode based on the given specs and add it to the end of the
   /// list of children.  Set 'instantiatingLoadedScene' to true if this is
   /// part of the instantiation of 'loadedScene'
   SceneNode* ChildCreate(JSONValue* newSceneNodeSpecs, bool instantiatingLoadedScene = false);

   /// Return a pointer to the first descandant found with the given name.
   /// If no matching node is found, return NULL.
   SceneNode* DescendantGetByName(const char* descendantName);

   /// Return true if the given node is a descendant of this one.
   bool SceneNodeDescendantCheck(SceneNode* possibleDescendant);

   /// Set how the object should be automatically rotated, if at all.
   void BillboardModeSet(BillboardMode _billboardMode) { billboardMode = _billboardMode; }
   /// Return how the object should be automatically rotated, if at all.
   BillboardMode BillboardModeGet() { return billboardMode; }

   /// Set the depth-sort priority if this node.  Depth-sorted nodes of
   /// numerically higher priority are drawn after depth-sorted nodes of lower
   /// priority.  Depth-sorted nodes of the same priority are sorted by depth.
   /// The default priority is 0.
   void DepthSortPrioritySet(float _depthSortPriority) { depthSortPriority = _depthSortPriority; }
   /// See 'DepthSortPrioritySet'.
   float DepthSortPriorityGet() { return depthSortPriority; }

   /// Return the filename of the sprite resource file to use for this node.
   /// Return NULL if this node currently has no sprite resource file.
   const char* SpriteResourceFilenameGet();
   /// Set the filename of the sprite resource file to use for this node and
   /// its descendants.  Return NULL if this node currently has no sprite
   /// resource file.
   void SpriteResourceFilenameSet(const char* _spriteResourceFilename);

   /// Set the font to be used when drawing text.  Set to NULL to make the node
   /// use its parent's font.
   void FontSet(Font* _font);
   /// Set the font to be used when drawing text.
   void FontSet(const char* filename);
   /// Return the font to use for this node.  If a font is not defined for the
   /// node, this will defer to the node's ancestors.  If there is no font
   /// defined and no parent, NULL will be returned.
   Font* FontGet();

   /// Refresh the text for the node.  Call this after a language change.
   virtual void TextRefresh();

   /// Add any drawables which should currently be drawn to the given
   /// collection.
   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode) { (void)drawables; (void)drawablesGatherMode; }

   /// Return the bounding sphere of the drawables of this node in local
   /// coordinates.  This does not necessarily include those of this node's
   /// children.  Return a sphere of radius 0 if this information is not
   /// available.
   virtual Sphere DrawablesBoundingSphereGet();

   /// Set the scale which should be applied to bounding volumes, like bounding
   /// spheres, for culling purposes.
   void BoundingVolumeScaleSet(float _boundingVolumeScale) { boundingVolumeScale = _boundingVolumeScale; }
   /// Return the scale which should be applied to bounding volumes, like
   /// bounding spheres, for culling purposes.
   float BoundingVolumeScaleGet() { return boundingVolumeScale; }

   /// Return true if view frustum culling should be used with this object.
   bool ViewFrustumCullingEnabledCheck() { return viewFrustumCullingEnabled; }
   /// Set whether view frustum culling should be used with this object.
   void ViewFrustumCullingEnabledSet(bool _viewFrustumCullingEnabled) { viewFrustumCullingEnabled = _viewFrustumCullingEnabled; }

   /// Perform a ray intersection test on the geometry of this object.  The ray
   /// should be in world space.  If 'recursive' is true, apply the test
   /// recursively to the descendants.  This is not necessarily implemented by
   /// all SceneNode types.
   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// See 'loadedScene'.  This should be called before Init.
   void LoadedSceneSet(Scene* _loadedScene) { loadedScene = _loadedScene; }
   /// See 'loadedScene'.
   Scene* LoadedSceneGet() { return loadedScene; }

   /// Call before Init if this node should only exist for preloading purposes.
   void PreloadOnlySet(bool _preloadOnly) { preloadOnly = _preloadOnly; }
   /// Return true if this node exists only for preloading purposes.
   bool PreloadOnlyCheck() { return preloadOnly; }

   /// Return true if this node's children form an LOD group.
   bool LODGroupCheck() { return lodGroup; }
   /// Set whether this node's children form an LOD group.
   void LODGroupSet(bool _lodGroup) { lodGroup = _lodGroup; }
   /// If this value is greater than 0 and this node is an LOD group, don't
   /// draw any of this node's children if the distance from the camera is
   /// greater than this amount.
   float LODGroupDrawDistanceMaxGet() { return lodGroupDrawDistanceMax; }
   /// See 'LodGroupDrawDistanceMaxGet'.
   void LODGroupDrawDistanceMaxSet(float _lodGroupDrawDistanceMax) { lodGroupDrawDistanceMax = _lodGroupDrawDistanceMax; }
   /// Don't draw this version if the distance to the camera is less than this
   /// amount.  Try to draw a higher-quality version instead.
   float LODLevelDrawDistanceMinGet() { return lodLevelDrawDistanceMin; }
   /// See 'LODLevelDrawDistanceMinGet'.
   void LODLevelDrawDistanceMinSet(float _lodLevelDrawDistanceMin) { lodLevelDrawDistanceMin = _lodLevelDrawDistanceMin; }

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNode(); }

   /// Cached absolute transform of the scene node for internal use only.
   Matrix43 transformAbsoluteCached;
   /// Cached value for whether the node should be displayed.  This is for
   /// internal use only.
   bool visibleEffectiveCached;
   /// Cached bounding sphere of the drawables in world space.  This is for
   /// internal use only.
   Sphere drawablesBoundingSphereAbsoluteCached;

private:
   /// Name of this node, if any.
   const char* name;
   /// Parent of this node, if any.
   SceneNode* parent;
   /// True if this node exists only for preloading purposes.
   bool preloadOnly;
   /// If this SceneNode was created by instantiating a Scene loaded from a
   /// file, this is a pointer to that Scene.  This is used for reference
   /// counting purposes.
   Scene* loadedScene;
   /// SceneInstance of which this node is currently a part.  If this is NULL,
   /// defer to the parent.
   SceneInstance* sceneInstance;
   /// False if this node should be hidden, independently of its ancestors
   /// and descendants.
   bool visibleSpecific;
   /// False if this node has been set explicitly so that it and its
   /// descendants should be hidden as a group.
   bool visibleHierarchical;
   /// False if this node should be excluded for collision detection,
   /// independently of its ancestors and descendants.
   bool collidableSpecific;
   /// False if this node has been set explicitly so that it and its
   /// descendants should be excluded for collision detection as a group.
   bool collidableHierarchical;
   /// Local position of the node.
   Point3F position;
   /// Local rotation of the node.
   Quaternion rotation;
   /// Local scale of the node.
   Point3F scale;
   /// True if this node's local transform should be considered relative to its
   /// parent and false if the local transform should be treated as also the
   /// world transform.
   bool relativeToParent;
   /// How the object should be automatically rotated, if at all.
   BillboardMode billboardMode;
   /// Depth-sorted nodes of numerically higher priority are drawn after
   /// depth-sorted nodes of lower priority.  Depth-sorted nodes of the same
   /// priority are sorted by depth.  The default priority is 0.
   float depthSortPriority;
   /// Scale which should be applied to bounding volumes, like bounding spheres,
   /// for culling purposes.
   float boundingVolumeScale;
   /// True if view frustum culling should be used with this object.
   bool viewFrustumCullingEnabled;
   /// Filename of the sprite resource file to use for this node and its
   /// descendants.
   const char* spriteResourceFilename;
   /// True if 'font' should be unloaded when this object no longer needs it.
   bool fontUnload;
   /// Font with which to draw text.  If this is NULL, use the parent's font.
   Font* font;
   /// True if this node's children form an LOD group.
   bool lodGroup;
   /// If this value is greater than 0 and this node is an LOD group, don't draw
   /// any of this node's children if the distance from the camera is greater
   /// than this amount.
   float lodGroupDrawDistanceMax;
   /// Don't draw this version if the distance to the camera is less than this
   /// amount.  Try to draw a higher-quality version instead.
   float lodLevelDrawDistanceMin;
   /// Children of this node.
   Table<SceneNode*> children;

   friend class SceneInstance;
};

//==============================================================================

/// SceneNodeRayIntersectResult helps facilitate ray intersection tests and
/// report results.  Call Reset before each test.  Initialize
/// 'intersectDistance' before each test to the maximum distance you want to
/// consider.
class SceneNodeRayIntersectResult
{
public:
   /// True if an intersection has been found.
   bool intersectFound;
   /// Distance to the nearest detected intersection.  Before performing each
   /// intersect test, initialize this to the maximum distance you want to
   /// consider.
   float intersectDistance;
   /// Location of the nearest intersection in world space.
   Point3F intersectPosition;
   /// Normal of the surface at the nearest intersection in world space.
   Point3F intersectNormal;
   /// SceneNode with which the nearest intersection was detected.
   SceneNode* intersectSceneNode;

   SceneNodeRayIntersectResult()
   {
      Reset();
   }

   /// Call this before each intersect test.
   void Reset()
   {
      intersectFound = false;
      intersectSceneNode = NULL;
   }
};

//==============================================================================

/// Return the billboard mode for the given name.  Return
/// BILLBOARD_MODE_INVALID if unsuccessful.
SceneNode::BillboardMode BillboardModeStringToCode(const char* name);
/// Return a string representation of the given billboard mode.  Return NULL if
/// unsuccessful.
const char* BillboardModeCodeToString(SceneNode::BillboardMode code);

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODE_H__
