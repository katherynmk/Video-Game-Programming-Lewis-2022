#ifndef __FROG__DUCK__SCENENODEDETAILMESHESRANDOM_H__
#define __FROG__DUCK__SCENENODEDETAILMESHESRANDOM_H__

#include "FrogMemory.h"
#include "Duck/SceneNodeDetailMeshes.h"

namespace Webfoot {
namespace Duck {

class SceneNodeDetailMeshesRandomGridCell;
class SceneNodeDetailMeshesRandomGridCellDetailMesh;
class SceneNodeDetailMeshesRandomDetailMeshSettings;
class SceneNodeDetailMeshesRandomDetailMeshLODSettings;
class SceneNodeMesh;
class SceneNodeTerrain;

/// Number for randomizing the detail meshes.  This can be used to add variety
/// between levels and among different uses of this class in a single level,
/// but it should not be counted on to produce a consistent result, especially
/// over long periods of time.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_SEED_KEY "Seed"
/// Within a cell, detail meshes are arranged in a grid.  This is the space
/// between each instance along each axis before random perturbation.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_SPACING_KEY "Spacing"
/// Random deviation applied to the position of the detail mesh instances
/// in x and y.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_SPACING_DEVIATION_KEY "SpacingDeviation"
/// Horizontal size of a grid cell, both for north/south and east/west.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_CELL_SIZE_HORIZONTAL_KEY "CellSizeHorizontal"
/// Region within which objects can be placed.  If a splatmap is provided, this
/// also gives the region of terrain over which the splatmap is stretched and
/// applied.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_BOUNDS_HORIZONTAL_KEY "BoundsHorizontal"
/// Set of detail meshes to be placed by this system.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESHES_KEY "DetailMeshes"

/// Filename of the scene with the detail mesh.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_SCENE_FILENAME_KEY "SceneFilename"
/// Minimum value for the random scale of detail object instances applied to X,
/// Y, and Z.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_SCALE_MIN_KEY "ScaleMin"
/// Maximum value for the random scale of detail object instances applied to X,
/// Y, and Z.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_SCALE_MAX_KEY "ScaleMax"
/// True if the detail objects should be rotated to match the slope of the
/// terrain.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_ROTATE_TO_NORMAL_KEY "RotateToNormal"
/// Random deviation in the rotation of the detail objects from the local up
/// direction.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_TILT_ANGLE_DEVIATION_KEY "TiltAngleDeviation"
/// Bitmap for determining the placement of the detail objects.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_SPLATMAP_KEY "Splatmap"
/// Multiplied by the splatmap value when determining the placement of detail
/// objects.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_DENSITY_COEFFICIENT_KEY "DensityCoefficient"
/// Set of specific meshes to be placed by this system, grouped by LOD.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_LOD_GROUP_KEY "LODGroup"

/// Name of the mesh node within the scene file to be used for this LOD.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_LOD_MESH_NODE_NAME_KEY "MeshNodeName"
/// Number of rows of grid cells in the ring buffer that should be used for
/// this level of detail.  The number of columns would be the same.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_DETAIL_MESH_LOD_CELL_ROW_COUNT_KEY "CellRowCount"

//==============================================================================

/// Do not count on this system to provide consistent placement or any other
/// random characteristics of objects, especially over long periods of time.
///
/// Scene node for managing a set of detail meshes which are placed randomly.
/// The region in which the mesh instances should be placed is broken up into a
/// grid.  All instances of an object in a given grid cell are LOD'd together.
/// The cells are internally set up in a ring buffer so the system doesn't need
/// to keep every possible instance in memory at the same time.  Instead, cells
/// close to the camera are refreshed as the camera moves.  To reduce the
/// impact on frame rate, it does not necessarily refresh all relevant cells on
/// a given update.  This has the side effect that groups of detail meshes will
/// pop in and out in groups.  This system is intended for things like grass on
/// large levels, where you wouldn't want to maintain a complete list of all
/// instances of a detail object, either on disk or in RAM.
class SceneNodeDetailMeshesRandom : public SceneNodeDetailMeshes
{
public:
   SceneNodeDetailMeshesRandom();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   /// Set the coefficient applied to 'detailMeshSpacing' and
   /// 'detailMeshSpacingDeviation' for lowering the number of mesh instances.
   void DetailMeshSpacingCoefficientSet(float _detailMeshSpacingCoefficient);
   /// Return the coefficient applied to 'detailMeshSpacing' and
   /// 'detailMeshSpacingDeviation' for lowering the number of mesh instances.
   /// This should ideally be called before calling 'DetailMeshesSet'.
   float DetailMeshSpacingCoefficientGet() { return detailMeshSpacingCoefficient; }

   /// Set the coefficient applied to 'cellRowCount' for lowering the
   /// distance at which meshes are drawn.
   void CellRowCountCoefficientSet(float _cellRowCountCoefficient);
   /// Return the coefficient applied to 'cellRowCount' for lowering the
   /// distance at which meshes are drawn.
   float CellRowCountCoefficientGet() { return cellRowCountCoefficient; }

   /// Set up the detail meshes based on the given specifications and position
   /// them on the given terrain.  For now, this assumes that this node and the
   /// terrain node have the same absolute transform.
   void DetailMeshesSet(JSONValue* specifications, SceneNodeTerrain* _sceneNodeTerrain);

   virtual void RefreshFull();

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeDetailMeshesRandom(); }

   typedef SceneNodeDetailMeshes Inherited;

protected:
   /// Helper function to the different forms of Init.
   void InitHelper();
   /// Clean up the current set of detail meshes.
   void DetailMeshesClear();
   /// Populate cells with detail meshes.  Ordinarily, this is done over
   /// multiple frames, but it will refresh all relevant cells if
   /// 'forceRefreshAll' is true.
   void CellsRefresh(bool forceRefreshAll);
   /// Populate the given grid cell.  Return true if time-consuming changes
   /// were made.
   bool CellInstancesRefresh(Point2I cellCoordinates, const Point3F& cameraPosition);

   /// Comparator for establishing the order in which cells should be updated.
   static bool GridCellUpdateOrderComparator(const Point2I& a, const Point2I& b);

   /// Number for randomizing the detail meshes.  This can be used to add variety
   /// between levels and among different uses of this class in a single level,
   /// but it should not be counted on to produce a consistent result, especially
   /// over long periods of time.
   unsigned int seed;
   /// Number of rows of grid cells in the ring buffer.  The number of columns
   /// is the same number.  This is determined by the highest number requested
   /// for a level of detail.
   int ringBufferCellRowCountFull;
   /// Horizontal size of a grid cell, both for north/south and east/west.
   float cellSizeHorizontal;
   /// Within a cell, detail meshes are arranged in a grid.  This is the space
   /// between each instance along each axis before random perturbation.
   float detailMeshSpacing;
   /// Random deviation applied to the position of the detail mesh instances
   /// in x and y.
   float detailMeshSpacingDeviation;
   /// Coefficient applied to 'detailMeshSpacing' and
   /// 'detailMeshSpacingDeviation' for lowering the number of mesh instances.
   float detailMeshSpacingCoefficient;
   /// Coefficient applied to 'cellRowCount' for lowering the distance at
   /// which meshes are drawn.
   float cellRowCountCoefficient;
   /// Terrain on which the detail objects should be placed.
   SceneNodeTerrain* sceneNodeTerrain;
   /// Region within which objects can be placed.
   Box2F boundsHorizontal;
   /// Pool of cells to be populated with detail mesh instances close to the
   /// camera.
   Table<SceneNodeDetailMeshesRandomGridCell*> ringBufferCells;
   /// Since the number of grid cells that can be refreshed on a given frame is
   /// limited, update the grid cells in an order that prioritizes those
   /// closest to the camera.  This gives an ordered list of offsets from the
   /// southwest-most cell that should be populated.
   Table<Point2I> gridCellUpdateOrder;
   /// Collection of settings for detail objects to be placed.
   Table<SceneNodeDetailMeshesRandomDetailMeshSettings*> detailMeshesSettings;

   friend class SceneNodeDetailMeshesRandomGridCell;
   friend class SceneNodeDetailMeshesRandomGridCellUpdateOrderComparator;
};

//==============================================================================

class SceneNodeDetailMeshesRandomGridCell
{
public:
   void Init(SceneNodeDetailMeshesRandom* ownerNode, int instanceCountMax);
   void Deinit();

   /// Hide any detail meshes for this cell.
   void Hide();

protected:
   /// Coordinates that were most recently used to populate this cell.
   Point2I cellCoordiantes;
   /// True if this cell is not currently populated.
   bool empty;
   /// Data for this cell which is specific to possible detail objects.
   Table<SceneNodeDetailMeshesRandomGridCellDetailMesh*> cellDetailMeshes;

   friend class SceneNodeDetailMeshesRandom;
};

//==============================================================================

/// Data for a given detail object, which may involve a separate representation
/// for each level of detail.
class SceneNodeDetailMeshesRandomGridCellDetailMesh
{
public:
   SceneNodeDetailMeshesRandomGridCellDetailMesh();

   void Init(SceneNodeDetailMeshesRandom* ownerNode, int instanceCountMax, SceneNodeDetailMeshesRandomDetailMeshSettings* _detailMeshSettings);
   void Deinit();

   /// Hide this detail mesh.
   void Hide();

protected:
   /// Add a detail mesh for the next highest LOD.
   void LODDetailMeshAdd(SceneNode* newParent, int lodNumber, SceneNode* meshSceneRootNode, int instanceCountMax);

   /// Collection of detail mesh nodes indexed by LOD.
   Table<SceneNodeMesh*> lodDetailMeshNodes;
   /// LOD level for which this cell was most recently populated.
   int lodNumber;
   /// Most recently calculated ideal LOD number for this detail object for
   /// this cell.  It is not necessarily the same as 'lodNumber'.
   int lodNumberIdeal;
   /// Settings which apply to this detail mesh.
   SceneNodeDetailMeshesRandomDetailMeshSettings* detailMeshSettings;

   friend class SceneNodeDetailMeshesRandom;
};

//==============================================================================

/// Properties specific to a given detail object, of which there may be
/// multiple versions for different LODs.
class SceneNodeDetailMeshesRandomDetailMeshSettings
{
public:
   SceneNodeDetailMeshesRandomDetailMeshSettings();

   bool Init(JSONValue* specs, SceneNodeDetailMeshesRandom* ownerNode);
   void Deinit();

protected:
   /// Filename of the scene with the detail mesh for this LOD.
   const char* sceneFilename;
   /// Minimum value for the random scale of detail object instances applied to X,
   /// Y, and Z.
   float scaleMin;
   /// Maximum value for the random scale of detail object instances applied to X,
   /// Y, and Z.
   float scaleMax;
   /// True if the detail objects should be rotated to match the slope of the
   /// terrain.
   bool rotateToNormal;
   /// Random deviation in the rotation of the detail objects from the local up
   /// direction.
   float tiltAngleDeviation;
   /// Bitmap for determining the placement of the detail objects of this type.
   Bitmap* splatmap;
   /// Multiplied by the splatmap value when determining the placement of detail
   /// objects.
   float densityCoefficient;
   /// Preloaded instance of the scene.
   SceneInstance* preloadedSceneInstance;
   /// Settings specific to a given LOD of a given detail object.
   Table<SceneNodeDetailMeshesRandomDetailMeshLODSettings*> lods;

   friend class SceneNodeDetailMeshesRandom;
   friend class SceneNodeDetailMeshesRandomGridCellDetailMesh;
};

//==============================================================================

/// Properties specific to a given level-of-detail of a given detail mesh.
class SceneNodeDetailMeshesRandomDetailMeshLODSettings
{
public:
   SceneNodeDetailMeshesRandomDetailMeshLODSettings();

   bool Init(JSONValue* specs, SceneNodeDetailMeshesRandom* ownerNode);
   void Deinit();

protected:
   /// Number of rows of grid cells in the ring buffer that should be used for
   /// this level of detail.  The number of columns would be the same.
   int cellRowCount;
   /// Name of the mesh node within the scene file to be used for this LOD.
   const char* meshNodeName;
   /// True if a warning has already been given about failing to find the node
   /// specified by 'meshNodeName'.
   bool meshNodeNameWarningGiven;
   /// True if a warning has already been given about this node's transform not
   /// being identity.
   bool meshNodeTransformWarningGiven;

   friend class SceneNodeDetailMeshesRandom;
   friend class SceneNodeDetailMeshesRandomGridCellDetailMesh;
};

//==============================================================================

/// Functor for establishing the order in which cells should be updated.  This
/// is used instead of a static function so we can provide more context for the
/// comparison.
class SceneNodeDetailMeshesRandomGridCellUpdateOrderComparator
{
public:
   SceneNodeDetailMeshesRandomGridCellUpdateOrderComparator() { sceneNodeDetailMeshesRandom = NULL; }

   bool operator()(const Point2I& a, const Point2I& b);

   /// Object which is requesting the comparisons.
   SceneNodeDetailMeshesRandom* sceneNodeDetailMeshesRandom;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEDETAILMESHESRANDOM_H__
