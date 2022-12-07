#ifndef __FROG__DUCK__SCENENODETERRAINLAYERED_H__
#define __FROG__DUCK__SCENENODETERRAINLAYERED_H__

#include "FrogMemory.h"
#include "Box2.h"
#include "Box3.h"
#include "FileManager.h"
#include "Point2.h"
#include "Texture.h"
#include "Duck/Drawable.h"
#include "Duck/SceneNodeTerrain.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

class SceneNodeTerrainLayered;
class SceneNodeTerrainLayeredDrawable;
class SceneNodeTerrainLayeredLayer;
class SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer;

/// Filename of the terrain heightmap.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_HEIGHTMAP_KEY "Heightmap"
/// Horizontal space between adjacent heightmap vertices.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_TERRAIN_SCALE_HORIZONTAL_KEY "ScaleHorizontal"
/// Scale applied to the vertex heights.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_TERRAIN_SCALE_VERTICAL_KEY "ScaleVertical"
/// Set of layers to use for drawing a given terrain.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYERS_KEY "Layers"
/// Texture to be sampled for the base color of a layer.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYER_BASE_COLOR_TEXTURE_KEY "BaseColorTexture"
/// Scale of the texture so that 1|1 would repeat the texture over 1 world
/// unit by 1 world unit.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYER_SCALE_KEY "Scale"
/// Metallic value of a terrain layer.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYER_METALLIC_KEY "Metallic"
/// Roughness value of a terrain layer.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYER_ROUGHNESS_KEY "Roughness"
/// True if the alpha channel of the base color texture contains roughness.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYER_ROUGHNESS_IN_BASE_COLOR_ALPHA_KEY "RoughnessInBaseColorAlpha"
/// Base specular reflectivity for non-metals when the angle of the light is
/// perpendicular to the surface normal.  (Fresnel0 for dielectric materials)
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_LAYER_DIELECTRIC_SPECULAR_KEY "DielectricSpecular"
/// Texture for controling to what extent different layers apply at different
/// locations.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_SPLATMAP_KEY "Splatmap"
/// True if a splatmap channel contains occlusion data.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_OCCLUSION_IN_SPLATMAP_KEY "OcclusionInSplatmap"
/// Number rows (and columns) of quads to draw in a single draw call.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_BATCH_ROW_COUNT_KEY "BatchRowCount"
/// Within this distance of the center of the quadtree node's bounding sphere,
/// quality should be maximized.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_QUALITY_MAX_DISTANCE_KEY "QualityMaxDistance"

//==============================================================================

/// Quadtree data structure to help with culling and LOD.
class SceneNodeTerrainLayeredQuadtreeNode
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

   SceneNodeTerrainLayeredQuadtreeNode();

   void Deinit();

   /// Return true if either this node or one of its descendants is currently
   /// marked for drawing.
   bool NodeOrDescendantMarkedForDrawingCheck();

   /// Scene node which owns this quadtree node.
   SceneNodeTerrainLayered* sceneNodeTerrainLayered;
   /// Which corner of the parent node this node occupies.
   Corner cornerWithinParent;
   /// Node of which this node is a child.
   SceneNodeTerrainLayeredQuadtreeNode* parent;
   /// Child nodes for each corner.
   SceneNodeTerrainLayeredQuadtreeNode* children[CORNER_COUNT];
   /// Cached pointers to neighboring nodes at the same depth in the tree.
   SceneNodeTerrainLayeredQuadtreeNode* neighbors[DIRECTION_CARDINAL_COUNT];
   /// Bounds of the vertex indices (inclusive) in the east/west and
   /// north/south axes.  Here, 'width' and 'height' refers to the number of
   /// indices for that axis, excluding the influence of LOD.  For instance,
   /// if the vertices involved on the x axis were 0, 1, and 2, the 'width'
   /// would be 3.  If the indices within the bounds were 0, 1, 2, 3, and 4,
   /// but the LOD was 1, indices 1 and 3 might not be involved when directly
   /// drawing this node, but the width would still be 5.  Since these are
   /// squares, 'width' and 'height' should always be equal.
   Box2I vertexIndexBounds2D;
   /// Bounding box of the vertices within this node and its descendants.
   Box3F boundingBox;
   /// Bounding sphere of the node.
   Sphere boundingSphere;
   /// True if this node should be drawn, as opposed to its children or one of
   /// its ascestors.
   bool drawAtThisLevel;
   /// Mipmap-style LOD number.  0 is maximum quality.  As this number
   /// increases, quality decreases.
   int lod;
   /// Buffer with cached vertex data.
   SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer* vertexBuffer;
};

//==============================================================================

/// Scene node for drawing terrain with layered texturing and adaptive level of
/// detail.
///
/// The geometry of the terrain begins with a heightmap, and normals are
/// generated from this.  The heightmap must be square, and its edge sizes must
/// be a power-of-2 plus 1.  (513x513, 1025x1025, etc.)  The heightmap must be
/// provided as a 16-bit grayscale PNG with no alpha (single channel).
///
/// Parts of the terrain which are closer to the viewer can be drawn with
/// greater detail than parts of the terrain which are farther away.  This is
/// facilitated by breaking up the heightmap using a quadtree.  The leaves of
/// the quadtree represent the highest quality (LOD 0).  Each node uses
/// the same number of triangles to draw its territory, but each covers the all
/// the terrain of its children.
///
/// Extra work is done internally to avoid gaps in the terrain at the borders
/// between different levels of detail.  The system tries to enforce the rule
/// that adjacently drawn patches of terrain are no more than 1 LOD apart.
/// From here, when you have a more detailed patch next to a less detailed
/// patch, the vertex heights of the odd vertices on the edge of the more
/// detailed patch are adjusted to match the midpoints of the triangle edges on
/// the edge of the less detailed patch.  This closes the gaps.  View frustum
/// culling is also used internally at the level of nodes to reduce waste.
class SceneNodeTerrainLayered : public SceneNodeTerrain
{
public:
   SceneNodeTerrainLayered();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet();
   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// Load the height data from the given bitmap.
   void TerrainLoad(const char* filename, FileManager* fileManager = theFiles);
   /// Set the terrain based on the given specifications.
   void TerrainSet(JSONValue* terrainSpecifications);
   /// Return the height of the given vertex.
   float VertexHeightGet(int x, int y);
   /// Return the height of the terrain at the given horizontal position in
   /// local coordanates.
   virtual float TerrainHeightGet(const Point2F& position);
   /// Return the normal of the given vertex.
   Point3F VertexNormalGet(int x, int y);
   /// Return the normal of the terrain at the given horizontal position in
   /// local coordanates.
   virtual Point3F TerrainNormalGet(const Point2F& position);
   /// Return the space between vertices in X and Y.
   float TerrainScaleHorizontalGet() { return terrainScaleHorizontal; }
   /// Return the horizontal bounds of the terrain in local coordinates.
   virtual Box2F TerrainBoundsHorizontalGet();
   /// Return the offset used to lower the displayed level of detail.
   int LODOffsetGet() { return lodOffset; }
   /// Set the offset used to lower the displayed level of detail.  This must
   /// be greater than or equal to 0.
   void LODOffsetSet(int _lodOffset);

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeTerrainLayered(); }

   /// Refresh the bounding volumes based on the terrain height data.
   void BoundingVolumesRefresh();

   typedef SceneNodeTerrain Inherited;

protected:
   /// Helper function for cleanup.
   void TerrainClear();
   /// Recursive function for building the quadtree.
   SceneNodeTerrainLayeredQuadtreeNode* QuadtreeBuildHelper(SceneNodeTerrainLayeredQuadtreeNode* _parentQuadtreeNode,
      SceneNodeTerrainLayeredQuadtreeNode::Corner _cornerWithinParent, Box2I vertexIndexBounds2D, int* _lod);
   /// Refresh the bounding volumes based on the terrain height data.
   void BoundingVolumesRefreshHelper(SceneNodeTerrainLayeredQuadtreeNode* quadtreeNode);
   /// Set the neighbor pointers of the quadtree nodes.
   void QuadtreeNeighborsSet(SceneNodeTerrainLayeredQuadtreeNode* quadtreeNode);
   /// Return the node at the given LOD with index bounds which begin at the given point.
   SceneNodeTerrainLayeredQuadtreeNode* QuadtreeNodeGetByIndexAndLOD(int minX, int minY, int lod);
   /// Compute and return which node would be the neighbor of the given node in
   /// the given direction at the same LOD.
   SceneNodeTerrainLayeredQuadtreeNode* NeighborCompute(SceneNodeTerrainLayeredQuadtreeNode* quadtreeNode, Direction direction);
   /// Determine which nodes of the tree should be drawn.
   void FrameLODSetup();
   /// Helper function for the first pass of setting up the quadtree for
   /// drawing.  Mark which nodes should be drawn.
   bool FrameLODSetupInit(SceneNodeTerrainLayeredQuadtreeNode* quadtreeNode, const Point3F& cameraLODPosition);
   /// Helper function for applying certain rules to which quadtree nodes can
   /// be drawn at the same time.
   ///
   /// The main goal of this function is for adjacently drawn patches of
   /// terrain to be no more than 1 LOD apart.  It should be called iteratively
   /// on the root such that if any pass through the tree makes any changes,
   /// the function will return true, and another pass should be made starting
   /// again from the root.
   bool FrameLODSetupConsistency(SceneNodeTerrainLayeredQuadtreeNode* quadtreeNode);
   /// Helper function to IntersectGridHelper for testing a single
   /// quad.  Return true if an intersection was found.
   static bool IntersectQuadHelperLayered(SceneNodeTerrain* sceneNodeTerrain, const Ray3& rayLocalSpace,
      SceneNodeRayIntersectResult* intersectResult, float& intersectDistanceScaled, const Point2I& southwestQuadIndices);
   /// Return an available vertex buffer for a quadtree node.
   SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer* QuadtreeNodeVertexBufferAvailableGet();
   /// Recycle a vertex buffer for a quadtree node.
   void QuadtreeNodeVertexBufferRecycle(SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer* vertexBuffer);

   /// Object for drawing the terrain.
   SceneNodeTerrainLayeredDrawable* drawable;
   /// Size of the heightmap in the horizontal axes.
   Point2I heightmapResolution;
   /// The space between vertices in X and Y.
   float terrainScaleHorizontal;
   /// Scale applied to the vertex heights.
   float terrainScaleVertical;
   /// Number rows (and columns) of quads to draw in a single draw call.
   int batchRowCount;
   /// Within this distance of the center of the quadtree node's bounding
   /// sphere, quality should be maximized.
   float qualityMaxDistance;
   /// True if a splatmap channel contains occlusion data.
   bool occlusionInSplatmap;
   /// Offset used to lower the displayed level of detail.
   int lodOffset;
   /// True if 'FrameLODSetup' has been called since the most recent update.
   bool frameLODSetupCalled;
   /// Array of height data.
   float* heightArray;
   /// Array of terrain vertex normals.
   Point3F* normalArray;
   /// Root of the quad tree to help with culling and LOD.
   SceneNodeTerrainLayeredQuadtreeNode* quadtreeRoot;
   /// Texture for controling to what extent different layers apply at
   /// different locations.
   Texture* splatmapTexture;
   /// Set of layers for texuring the terrain.
   Table<SceneNodeTerrainLayeredLayer*> layers;
   /// Collection of vertex buffers which are not currently assigned to a
   /// quadtree node.
   Table<SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer*> quadtreeNodeVertexBuffersAvailable;

   friend class SceneNodeTerrainLayeredDrawable;
   friend class SceneNodeTerrainLayeredQuadtreeNode;
};

//==============================================================================

/// Material used for the terrain.
class SceneNodeTerrainLayeredLayer
{
public:
   SceneNodeTerrainLayeredLayer();

   virtual void Init(JSONValue* specifications);
   virtual void Deinit();

   /// Texture to be sampled for the base color.
   Texture* baseColorTexture;
   /// Scale of the texture so that 1|1 would repeat the texture over 1 world
   /// unit by 1 world unit.
   Point2F scale;
   /// Metallic value of the layer material.
   float metallic;
   /// Roughness value of the layer material.
   float roughness;
   /// True if the alpha channel of the base color texture contains roughness.
   bool roughnessInBaseColorAlpha;
   /// Base specular reflectivity for non-metals when the angle of the light is
   /// perpendicular to the surface normal.  (Fresnel0 for dielectric materials)
   float dielectricSpecular;
};

//==============================================================================

/// Helper class for drawing layered, adaptive terrain in Duck.
class SceneNodeTerrainLayeredDrawable : public Drawable
{
public:
   SceneNodeTerrainLayeredDrawable();

   virtual void Init(SceneNodeTerrainLayered* _sceneNodeTerrainLayered);
   virtual void Deinit();

   virtual SceneNode* DrawableSceneNodeGet() { return sceneNodeTerrainLayered; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

   /// Create a new instance of a 'SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer'
   virtual SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer* QuadtreeNodeVertexBufferCreate() = 0;

protected:
   /// Helper function for calling the 'FrameLODSetup' function of the
   /// corresponding 'sceneNodeTerrainLayered'.
   void FrameLODSetup();
   /// Helper function for getting the 'heightArray' member of the
   /// corresponding 'sceneNodeTerrainLayered'.
   float* HeightArrayGet();
   /// Helper function for getting the 'normalArray' member of the
   /// corresponding 'sceneNodeTerrainLayered'.
   Point3F* NormalArrayGet();
   /// Helper function for getting the 'heightmapResolution' member of the
   /// corresponding 'sceneNodeTerrainLayered'.
   Point2I HeightmapResolutionGet();
   /// Helper function for getting the 'batchRowCount' member of the
   /// corresponding 'sceneNodeTerrainLayered'.
   int BatchRowCountGet();
   /// Helper funtion for getting the 'quadtreeRoot' member of the
   /// corresponding 'sceneNodeTerrainLayered'.
   SceneNodeTerrainLayeredQuadtreeNode* QuadtreeRootGet();
   /// Return the set of layers for texuring the terrain.
   Table<SceneNodeTerrainLayeredLayer*>* LayersGet();
   /// Return the texture for controling to what extent different layers apply
   /// at different locations.
   Texture* SplatmapTextureGet();
   /// Return true if a splatmap channel contains occlusion data.
   bool OcclusionInSplatmapCheck();
   /// Return an available vertex buffer for a quadtree node.
   SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer* QuadtreeNodeVertexBufferAvailableGet();

   /// SceneNodeTerrainLayered to which this drawable belongs.
   SceneNodeTerrainLayered* sceneNodeTerrainLayered;
};

//==============================================================================

/// This caches the vertex data for a single quadtree node so it doesn't need
/// to be computed and transferred to the GPU as often.
class SceneNodeTerrainLayeredQuadtreeNodeVertexBuffer
{
public:
   void Init() {}
   virtual void Deinit() {}
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODETERRAINLAYERED_H__
