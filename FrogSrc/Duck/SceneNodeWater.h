#ifndef __FROG__DUCK__SCENENODEWATER_H__
#define __FROG__DUCK__SCENENODEWATER_H__

#include "FrogMemory.h"
#include "Box2.h"
#include "Box3.h"
#include "FileManager.h"
#include "Point2.h"
#include "Texture.h"
#include "Duck/Drawable.h"
#include "Duck/SceneNode.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

class SceneNodeWaterDrawable;
class SceneNodeWaterWave;
class SceneNodeWaterWaveGroup;

/// True if the top surface of the water should be included.
#define DUCK_SCENE_NODE_WATER_SURFACE_TOP_INCLUDED_KEY "SurfaceTopIncluded"
/// True if the bottom surface of the water should be included.
#define DUCK_SCENE_NODE_WATER_SURFACE_BOTTOM_INCLUDED_KEY "SurfaceBottomIncluded"
/// True if vertical faces should be drawn to help enclose the water volume.
#define DUCK_SCENE_NODE_WATER_CURTAINS_INCLUDED_KEY "CurtainsIncluded"
/// True if a floor should be drawn to help enclose the water volume.
#define DUCK_SCENE_NODE_WATER_FLOOR_INCLUDED_KEY "FloorIncluded"
/// Vertical position of the resting water surface.
#define DUCK_SCENE_NODE_WATER_SURFACE_REST_HEIGHT_KEY "SurfaceRestHeight"
/// Vertical position of the floor.
#define DUCK_SCENE_NODE_WATER_FLOOR_HEIGHT_KEY "FloorHeight"
/// Color to use for the top of the water surface.
#define DUCK_SCENE_NODE_WATER_SURFACE_TOP_BASE_COLOR_KEY "SurfaceTopBaseColor"
/// PBR roughness value for the top water surface.
#define DUCK_SCENE_NODE_WATER_SURFACE_TOP_ROUGHNESS_KEY "SurfaceTopRoughness"
/// Texture applied to the surface as a normal map.
#define DUCK_SCENE_NODE_WATER_SURFACE_NORMAL_TEXTURE_KEY "SurfaceNormalTexture"
/// Horizontal scale applied to the normal texture before wave deformation is
/// applied.
#define DUCK_SCENE_NODE_WATER_SURFACE_NORMAL_TEXTURE_SCALE_KEY "SurfaceNormalTextureScale"
/// Coefficient for the influence of the normal map.
#define DUCK_SCENE_NODE_WATER_SURFACE_NORMAL_TEXTURE_WEIGHT_KEY "SurfaceNormalTextureWeight"
/// Linear speed of the normal map in world units per second before wave
/// deformation is applied.
#define DUCK_SCENE_NODE_WATER_SURFACE_NORMAL_TEXTURE_SPEED_KEY "SurfaceNormalTextureSpeed"
/// Angle in degrees for determining the direction of the normal map.  0
/// degrees corresponds to +y, 90 degrees corresponds to -x, and so on.
#define DUCK_SCENE_NODE_WATER_SURFACE_NORMAL_TEXTURE_VELOCITY_ANGLE_KEY "SurfaceNormalTextureVelocityAngle"
/// Size of the heightmap in the horizontal axes.
#define DUCK_SCENE_NODE_WATER_HEIGHTMAP_EDGE_SIZE_KEY "HeightmapEdgeSize"
/// Horizontal space between vertices in X and Y at maximum quality.
#define DUCK_SCENE_NODE_WATER_WATER_SCALE_HORIZONTAL_KEY "ScaleHorizontal"
/// Number rows (and columns) of quads to draw in a single draw call.
#define DUCK_SCENE_NODE_WATER_BATCH_ROW_COUNT_KEY "BatchRowCount"
/// Within this distance of the center of the quadtree node's bounding sphere,
/// quality should be maximized.
#define DUCK_SCENE_NODE_WATER_QUALITY_MAX_DISTANCE_KEY "QualityMaxDistance"
/// True if the bottom surface of the water should be included.
/// Collections of objects that generate waves.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUPS_KEY "WaveGroups"
/// Coefficient applied to the amplitudes of the waves in the group.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUP_AMPLITUDE_COEFFICIENT_KEY "AmplitudeCoefficient"
/// Coefficient applied to the lengths of the waves in the group.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUP_LENGTH_COEFFICIENT_KEY "LengthCoefficient"
/// Coefficient applied to the speeds of the waves in the group.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUP_SPEED_COEFFICIENT_KEY "SpeedCoefficient"
/// Coefficient applied to the steepness of the waves in the group.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUP_STEEPNESS_COEFFICIENT_KEY "SteepnessCoefficient"
/// Velocity angle offset applied to the waves in the group, given in degrees.
/// 0 degrees corresponds to +y, 90 degrees corresponds to -x, and so on.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUP_VELOCITY_ANGLE_OFFSET_COEFFICIENT_KEY "VelocityAngleOffset"
/// Members of a given wave group.
#define DUCK_SCENE_NODE_WATER_WAVE_GROUP_WAVES_KEY "Waves"
/// Maximum distance from the base value in world units.
#define DUCK_SCENE_NODE_WATER_WAVE_AMPLITUDE_KEY "Amplitude"
/// Distance between wave crests in world units.
#define DUCK_SCENE_NODE_WATER_WAVE_LENGTH_KEY "Length"
/// Speed at which the wave propagates in world units per second.
#define DUCK_SCENE_NODE_WATER_WAVE_SPEED_KEY "Speed"
/// Controls how steep the peaks of the waves should be.  Range: [0, 1]
#define DUCK_SCENE_NODE_WATER_WAVE_STEEPNESS_KEY "Steepness"
/// Angle in degrees for determining the direction of the wave.  0 degrees
/// corresponds to +y, 90 degrees corresponds to -x, and so on.
#define DUCK_SCENE_NODE_WATER_WAVE_VELOCITY_ANGLE_KEY "VelocityAngle"

//==============================================================================

/// Quadtree data structure to help with culling and LOD.
class SceneNodeWaterQuadtreeNode
{
public:
   /// Places for a node to be in the parent node.
   enum Corner
   {
      /// Not actually a corner.  This is the reserved for the root node.
      CORNER_ROOT = -1,
      /// Southwest node within a parent node.
      CORNER_SOUTHWEST,
      /// Northwest node within a parent node.
      CORNER_NORTHWEST,
      /// Northeast node within a parent node.
      CORNER_NORTHEAST,
      /// Southeast node within a parent node.
      CORNER_SOUTHEAST,
      CORNER_COUNT
   };

   /// Bit masks for different edges of a quadtree node.
   enum EdgeMask
   {
      /// Bit mask for the north edge.
      EDGE_MASK_NORTH = 1 << 0,
      /// Bit mask for the south edge.
      EDGE_MASK_SOUTH = 1 << 1,
      /// Bit mask for the west edge.
      EDGE_MASK_WEST = 1 << 2,
      /// Bit mask for the east edge.
      EDGE_MASK_EAST = 1 << 3
   };

   SceneNodeWaterQuadtreeNode();

   void Deinit();

   /// Return true if either this node or one of its descendants is currently
   /// marked for drawing.
   bool NodeOrDescendantMarkedForDrawingCheck();

   /// Which corner of the parent node this node occupies.
   Corner cornerWithinParent;
   /// Node of which this node is a child.
   SceneNodeWaterQuadtreeNode* parent;
   /// Child nodes for each corner.
   SceneNodeWaterQuadtreeNode* children[CORNER_COUNT];
   /// Cached pointers to neighboring nodes at the same depth in the tree.
   SceneNodeWaterQuadtreeNode* neighbors[DIRECTION_CARDINAL_COUNT];
   /// Bounds of the vertex indices (inclusive) in the east/west and
   /// north/south axes.  Here, 'width' and 'height' refers to the number of
   /// indices for that axis, excluding the influence of LOD.  For instance,
   /// if the vertices involved on the x axis were 0, 1, and 2, the 'width'
   /// would be 3.  If the indices within the bounds were 0, 1, 2, 3, and 4,
   /// but the LOD was 1, indices 1 and 3 might not be involved when directly
   /// drawing this node, but the width would still be 5.  Since these are
   /// squares, 'width' and 'height' should always be equal.
   Box2I vertexIndexBounds2D;
   /// Bounding box of the surface vertices within this node and its
   /// descendants.
   Box3F surfaceBoundingBox;
   /// Bounding sphere of the surface for this node.
   Sphere surfaceBoundingSphere;
   /// Bounding sphere of the curtains of this node.
   Sphere curtainsBoundingSphere;
   /// True if this node should be drawn, as opposed to its children or one of
   /// its ascestors.
   bool drawAtThisLevel;
   /// Mipmap-style LOD number.  0 is maximum quality.  As this number
   /// increases, quality decreases.
   int lod;
};

//==============================================================================

/// Scene node for drawing a water surface.
class SceneNodeWater : public SceneNode
{
public:
   SceneNodeWater();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet();
   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// Set the seater based on the given specifications.
   void WaterSet(JSONValue* waterSpecifications);
   /// Return the certical position of the resting water surface in local
   /// space.
   float SurfaceRestHeightGet() { return surfaceRestHeight; }
   /// Return the time used for animating the water surface.
   float WaterTimeGet() { return waterTime; }

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeWater(); }

   /// Refresh the bounding volumes based on the water height data.
   void BoundingVolumesRefresh();

   typedef SceneNode Inherited;

protected:
   /// Helper function for cleanup.
   void WaterClear();
   /// Recursive function for building the quadtree.
   SceneNodeWaterQuadtreeNode* QuadtreeBuildHelper(SceneNodeWaterQuadtreeNode* _parentQuadtreeNode,
      SceneNodeWaterQuadtreeNode::Corner _cornerWithinParent, Box2I vertexIndexBounds2D, int* _lod);
   /// Refresh the bounding volumes based on the water height data.
   void BoundingVolumesRefreshHelper(SceneNodeWaterQuadtreeNode* quadtreeNode);
   /// Set the neighbor pointers of the quadtree nodes.
   void QuadtreeNeighborsSet(SceneNodeWaterQuadtreeNode* quadtreeNode);
   /// Return the node at the given LOD with index bounds which begin at the given point.
   SceneNodeWaterQuadtreeNode* QuadtreeNodeGetByIndexAndLOD(int minX, int minY, int lod);
   /// Compute and return which node would be the neighbor of the given node in
   /// the given direction at the same LOD.
   SceneNodeWaterQuadtreeNode* NeighborCompute(SceneNodeWaterQuadtreeNode* quadtreeNode, Direction direction);
   /// Determine which nodes of the tree should be drawn.
   void FrameLODSetup();
   /// Helper function for the first pass of setting up the quadtree for
   /// drawing.  Mark which nodes should be drawn.
   bool FrameLODSetupInit(SceneNodeWaterQuadtreeNode* quadtreeNode, const Point3F& cameraLODPosition);
   /// Helper function for applying certain rules to which quadtree nodes can
   /// be drawn at the same time.
   ///
   /// The main goal of this function is for adjacently drawn patches of
   /// terrain to be no more than 1 LOD apart.  It should be called iteratively
   /// on the root such that if any pass through the tree makes any changes,
   /// the function will return true, and another pass should be made starting
   /// again from the root.
   bool FrameLODSetupConsistency(SceneNodeWaterQuadtreeNode* quadtreeNode);

   /// True if the top surface of the water should be included.
   bool surfaceTopIncluded;
   /// True if the bottom surface of the water should be included.
   bool surfaceBottomIncluded;
   /// True if vertical faces should be drawn to help enclose the water volume.
   bool curtainsIncluded;
   /// True if a floor should be drawn to help enclose the water volume.
   bool floorIncluded;
   /// Object for drawing the terrain.
   SceneNodeWaterDrawable* drawable;
   /// Vertical position of the resting water surface.
   float surfaceRestHeight;
   /// Vertical position of the floor.
   float floorHeight;
   /// Color to use for the top of the water surface.
   ColorRGBA8 surfaceTopBaseColor;
   /// PBR roughness value for the top water surface.
   float surfaceTopRoughness;
   /// Time used for animating the water surface.
   float waterTime;
   /// Maximum expected height of the water above or below its rest level.
   float surfaceAmplitudeMax;
   /// Size of the heightmap in the horizontal axes.
   int heightmapEdgeSize;
   /// The space between vertices in X and Y at maximum quality.
   float waterScaleHorizontal;
   /// Number rows (and columns) of quads to draw in a single draw call.
   int batchRowCount;
   /// Within this distance of the center of the quadtree node's bounding
   /// sphere, quality should be maximized.
   float qualityMaxDistance;
   /// Offset used to lower the displayed level of detail.
   int lodOffset;
   /// True if 'FrameLODSetup' has been called since the most recent update.
   bool frameLODSetupCalled;
   /// Texture applied to the surface as a normal map.
   Texture* surfaceNormalTexture;
   /// Horizontal scale applied to the normal texture before wave deformation
   /// is applied.
   Point2F surfaceNormalTextureScale;
   /// Coefficient for the influence of the normal map.
   float surfaceNormalTextureWeight;
   /// Movement of the surface normal texture in world units per second before
   /// wave deformation is applied.
   Point2F surfaceNormalTextureVelocity;
   /// Root of the quad tree to help with culling and LOD.
   SceneNodeWaterQuadtreeNode* quadtreeRoot;
   /// Groups of objects which generate waves.
   Table<SceneNodeWaterWaveGroup*> waveGroups;

   friend class SceneNodeWaterDrawable;
};

//==============================================================================

/// Helper class for a group of objects for generating waves.
class SceneNodeWaterWaveGroup
{
public:
   void Init(JSONValue* specifications);
   void Deinit();

   /// Coefficient applied to the amplitudes of the waves in the group.
   float amplitudeCoefficient;
   /// Coefficient applied to the lengths of the waves in the group.
   float lengthCoefficient;
   /// Coefficient applied to the speeds of the waves in the group.
   float speedCoefficient;
   /// Coefficient applied to the steepness of the waves in the group.
   float steepnessCoefficient;
   /// Velocity angle offset applied to the waves in the group, given in
   /// degrees.  0 degrees corresponds to +y, 90 degrees corresponds to -x, and
   /// so on.
   float velocityAngleOffset;
   /// Members of the group of waves.
   Table<SceneNodeWaterWave*> waves;
};

//==============================================================================

/// Helper class for generating waves.
class SceneNodeWaterWave
{
public:
   void Init(JSONValue* specifications);

   /// Maximum distance from the base value in world units.
   float amplitude;
   /// Distance between wave crests in world units.
   float length;
   /// Speed at which the wave propagates in world units per second.
   float speed;
   /// Controls how steep the peaks of the waves should be.  Range: [0, 1]
   float steepness;
   /// Angle in degrees for determining the direction of the wave.  0 degrees
   /// corresponds to +y, 90 degrees corresponds to -x, and so on.
   float velocityAngle;
};

//==============================================================================

/// Helper class for drawing a water surface in Duck.
class SceneNodeWaterDrawable : public Drawable
{
public:
   SceneNodeWaterDrawable();

   virtual void Init(SceneNodeWater* _sceneNodeWater);
   virtual void Deinit();

   virtual SceneNode* DrawableSceneNodeGet() { return sceneNodeWater; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

protected:
   /// Helper function for calling the 'FrameLODSetup' function of the
   /// corresponding 'sceneNodeWater'.
   void FrameLODSetup();
   /// Helper function for getting the 'surfaceTopIncluded' member of the
   /// corresponding 'sceneNodeWater'.
   bool SurfaceTopIncludedCheck();
   /// Helper function for getting the 'surfaceBottomIncluded' member of the
   /// corresponding 'sceneNodeWater'.
   bool SurfaceBottomIncludedCheck();
   /// Helper function for getting the 'curtainsIncluded' member of the
   /// corresponding 'sceneNodeWater'.
   bool CurtainsIncludedCheck();
   /// Helper function for getting the 'floorIncluded' member of the
   /// corresponding 'sceneNodeWater'.
   bool FloorIncludedCheck();
   /// Helper function for getting the 'surfaceRestHeight' member of the
   /// corresponding 'sceneNodeWater'.
   float SurfaceRestHeightGet();
   /// Helper function for getting the 'floorHeight' member of the
   /// corresponding 'sceneNodeWater'.
   float FloorHeightGet();
   /// Helper function for getting the 'surfaceTopBaseColor' member of the
   /// corresponding 'sceneNodeWater'.
   ColorRGBA8 SurfaceTopBaseColorGet();
   /// Helper function for getting the 'surfaceTopRoughness' member of the
   /// corresponding 'sceneNodeWater'.
   float SurfaceTopRoughnessGet();
   /// Helper function for getting the 'surfaceNormalTexture' member of the
   /// corresponding 'sceneNodeWater'.
   Texture* SurfaceNormalTextureGet();
   /// Helper function for getting the 'surfaceNormalTextureScale' member of
   /// the corresponding 'sceneNodeWater'.
   Point2F SurfaceNormalTextureScaleGet();
   /// Helper function for getting the 'surfaceNormalTextureWeight' member of
   /// the corresponding 'sceneNodeWater'.
   float SurfaceNormalTextureWeightGet();
   /// Helper function for getting the 'surfaceNormalTextureVelocity' member of
   /// the corresponding 'sceneNodeWater'.
   Point2F SurfaceNormalTextureVelocityGet();
   /// Helper function for getting the 'waterScaleHorizontal' member of the
   /// corresponding 'sceneNodeWater'.
   float WaterScaleHorizontalGet();
   /// Helper function for getting the 'heightmapEdgeSize' member of the
   /// corresponding 'sceneNodeWater'.
   int HeightmapEdgeSizeGet();
   /// Helper function for getting the 'batchRowCount' member of the
   /// corresponding 'sceneNodeWater'.
   int BatchRowCountGet();
   /// Helper funtion for getting the 'quadtreeRoot' member of the
   /// corresponding 'sceneNodeWater'.
   SceneNodeWaterQuadtreeNode* QuadtreeRootGet();
   /// Return the groups of objects which generate waves.
   Table<SceneNodeWaterWaveGroup*>* WaveGroupsGet();

   /// SceneNodeWater to which this drawable belongs.
   SceneNodeWater* sceneNodeWater;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEWATER_H__
