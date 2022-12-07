#ifndef __FROG__DUCK__SCENENODETERRAINTILED_H__
#define __FROG__DUCK__SCENENODETERRAINTILED_H__

#include "FrogMemory.h"
#include "Box2.h"
#include "Box3.h"
#include "FileManager.h"
#include "Point2.h"
#include "Duck/Drawable.h"
#include "Duck/SceneNodeTerrain.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

struct SceneNodeTerrainTiledCell;
class SceneNodeTerrainTiledDrawable;
class SceneNodeTerrainTiledTile;

/// Resolution of the terrain heightmap.
#define DUCK_SCENE_NODE_TERRAIN_TILED_HEIGHTMAP_RESOLUTION_KEY "HeightmapResolution"
/// Height of the vertices as an array of little-endian IEEE 754 floats encoded
/// as a UTF-8 RFC 4648 Base64 string.
#define DUCK_SCENE_NODE_TERRAIN_TILED_VERTEX_HEIGHTS_KEY "VertexHeights"
/// Horizontal length of a cell edge in world units.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TERRAIN_SCALE_HORIZONTAL_KEY "ScaleHorizontal"
/// Data for the cells of terrain as an array of structs encoded as a UTF-8 RFC
/// 4648 Base64 string.
#define DUCK_SCENE_NODE_TERRAIN_TILED_CELLS_KEY "Cells"
/// Set of tile textures and their properties available to a terrain.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILESET_KEY "Tileset"
/// How many cells a given tile type spans.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILE_SIZE_CELLS_KEY "SizeCells"
/// Path used to load the texture for a tile.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILE_TEXTURE_FILENAME_KEY "Filename"
/// Mapping of terrain tile indices to tile names.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILE_NUMBERS_KEY "TileNumbers"
/// Number of a tile used for a terrain cell.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILE_NUMBER_KEY "Number"
/// Name of a tile used for a terrain cell.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILE_NAME_KEY "Name"
/// Maximum valid number of tiles.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TILE_COUNT_MAX 300

/// Cell flag bitmask for whether cells should be split into two triangles
/// southwest-to-northeast rather than northwest-to-southeast.
#define DUCK_SCENE_NODE_TERRAIN_TILED_CELL_TYPE_MASK 0x04

//==============================================================================

/// SceneNode for drawing a terrain composed of textured tiles.  This is used to
/// draw levels ported from the original Land engine.  Textures may span a
/// single tile or multiple tiles.  Individual cells of terrain may be bisected
/// into triangles northwest-to-southeast or southwest-to-northeast.  Because
/// this is only intended for retro Land games, the lighting model and map sizes
/// are especially limited.
class SceneNodeTerrainTiled : public SceneNodeTerrain
{
public:
   SceneNodeTerrainTiled();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   virtual void DrawablesGather(Table<Drawable*>* drawables, DrawablesGatherMode drawablesGatherMode);
   virtual Sphere DrawablesBoundingSphereGet() { return boundingSphere; }
   virtual void Intersect(const Ray3& ray, SceneNodeRayIntersectResult* intersectResult, bool recursive = true);

   /// Set the terrain based on the given specifications.  Height and cell data
   /// for procedural levels can be provided through the optional parameters.
   void TerrainSet(JSONValue* terrainSpecifications,
      float* _heightArray = NULL, SceneNodeTerrainTiledCell* _cellArray = NULL);
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
   /// Return the cell struct for the given cell.
   SceneNodeTerrainTiledCell* CellGet(int x, int y);
   /// Return the space between vertices in X and Y.
   float TerrainScaleHorizontalGet() { return terrainScaleHorizontal; }
   /// Return the horizontal bounds of the terrain in local coordinates.
   virtual Box2F TerrainBoundsHorizontalGet();
   /// Return the index of the tile in the tileset with the given name.  Return
   /// -1 if unsuccessful.
   int TileIndexGetByName(const char* tileName);

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeTerrainTiled(); }

   typedef SceneNodeTerrain Inherited;

protected:
   /// Helper function to IntersectGridHelper for testing a single
   /// quad.  Return true if an intersection was found.
   static bool IntersectQuadHelperTiled(SceneNodeTerrain* sceneNodeTerrain, const Ray3& rayLocalSpace,
      SceneNodeRayIntersectResult* intersectResult, float& intersectDistanceScaled, const Point2I& southwestQuadIndices);

   /// Object for drawing the terrain.
   SceneNodeTerrainTiledDrawable* drawable;
   /// Size of the heightmap in the horizontal axes.
   Point2I heightmapResolution;
   /// The space between vertices in X and Y.
   float terrainScaleHorizontal;
   /// Bounding box of the terrain.
   Box3F boundingBox;
   /// Bounding sphere of the terrain.
   Sphere boundingSphere;
   /// Array of height data.
   float* heightArray;
   /// Array of terrain vertex normals.
   Point3F* normalArray;
   /// Array of cell data.
   SceneNodeTerrainTiledCell* cellArray;
   /// Set of tiles used to draw cells.
   Table<SceneNodeTerrainTiledTile*> tiles;

   friend class SceneNodeTerrainTiledDrawable;
};

//==============================================================================

/// Data for a single cell of terrain.
struct SceneNodeTerrainTiledCell
{
   /// 0000ITRR
   /// T=bisect type, RR=rotation, I=invert triangles (unused)
   uint8 flags;
   /// Index of the tile to use to draw this cell.
   uint16 tileIndex;
};

//==============================================================================

/// One item in the tile set.
class SceneNodeTerrainTiledTile
{
public:
   SceneNodeTerrainTiledTile();

   virtual void Init(const char* _name, JSONValue* specifications);
   virtual void Deinit();

   /// Name of the tile for coordinating with the map's texture numbers.
   const char* name;
   /// How many cells this tile type should span.
   Point2I sizeCells;
   /// Texture used to draw the tile.
   Texture* texture;
};

//==============================================================================

/// Helper class for drawing tiled terrain in Duck.
class SceneNodeTerrainTiledDrawable : public Drawable
{
public:
   SceneNodeTerrainTiledDrawable();

   virtual void Init(SceneNodeTerrainTiled* _sceneNodeTerrainTiled);
   virtual void Deinit();

   virtual SceneNode* DrawableSceneNodeGet() { return sceneNodeTerrainTiled; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

protected:
   /// Helper function for getting the 'heightArray' member of the
   /// corresponding 'SceneNodeTerrainTiled'.
   float* HeightArrayGet();
   /// Helper function for getting the 'normalArray' member of the
   /// corresponding 'SceneNodeTerrainTiled'.
   Point3F* NormalArrayGet();
   /// Helper function for getting the 'heightmapResolution' member of the
   /// corresponding 'SceneNodeTerrainTiled'.
   Point2I HeightmapResolutionGet();
   /// Return the set of tiles used to draw cells.
   Table<SceneNodeTerrainTiledTile*>* TilesGet();

   /// SceneNodeTerrainTiled to which this drawable belongs.
   SceneNodeTerrainTiled* sceneNodeTerrainTiled;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODETERRAINTILED_H__
