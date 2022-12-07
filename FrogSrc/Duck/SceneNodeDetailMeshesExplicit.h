#ifndef __FROG__DUCK__SCENENODEDETAILMESHESEXPLICIT_H__
#define __FROG__DUCK__SCENENODEDETAILMESHESEXPLICIT_H__

#include "FrogMemory.h"
#include "Duck/SceneNodeDetailMeshes.h"

namespace Webfoot {
namespace Duck {

class SceneNodeDetailMeshesExplicitGridCell;
class SceneNodeDetailMeshesExplicitGridCellDetailMesh;
class SceneNodeDetailMeshesExplicitDetailMeshSettings;
class SceneNodeDetailMeshesExplicitDetailMeshLODSettings;
class SceneNodeMesh;
class SceneNodeTerrain;

/// Horizontal size of a grid cell, both for north/south and east/west.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_CELL_SIZE_HORIZONTAL_KEY "CellSizeHorizontal"
/// Set of detail meshes to be placed by this system.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_DETAIL_MESHES_KEY "DetailMeshes"

/// Filename of the scene with the detail mesh.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_DETAIL_MESH_SCENE_FILENAME_KEY "SceneFilename"
/// Set of specific meshes to be placed by this system, grouped by LOD.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_DETAIL_MESH_LOD_GROUP_KEY "LODGroup"
/// Set of instances of a given detail mesh to be shown.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_DETAIL_MESH_INSTANCES_KEY "Instances"

/// Name of the mesh node within the scene file to be used for this LOD.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_DETAIL_MESH_LOD_MESH_NODE_NAME_KEY "MeshNodeName"
/// Number of rows of grid cells in the ring buffer that should be used for
/// this level of detail.  The number of columns would be the same.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_DETAIL_MESH_LOD_CELL_ROW_COUNT_KEY "CellRowCount"

//==============================================================================

/// Scene node for managing a set of detail meshes which are placed explicitly.
/// The region in which the mesh instances should be placed is broken up into a
/// grid.  All instances of an object in a given grid cell are LOD'd together.
/// The cells are internally set up in a ring buffer so the system doesn't need
/// to keep every possible instance in memory at the same time.  Instead, cells
/// close to the camera are refreshed as the camera moves.  To reduce the
/// impact on frame rate, it does not necessarily refresh all relevant cells on
/// a given update.  This has the side effect that groups of detail meshes will
/// pop in and out in groups.  This system is intended for things like plants in
/// a garden, where they need to be placed in specific ways, there are too many
/// to not use instancing, but not so many that they would need to be placed
/// randomly.  SceneNodeDetailMeshesExplicit should be used instead of this when
/// practical.
class SceneNodeDetailMeshesExplicit : public SceneNodeDetailMeshes
{
public:
   SceneNodeDetailMeshesExplicit();

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);
   virtual void Deinit();
   virtual void Update(unsigned int dt);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

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
   static SceneNode* Create() { return frog_new SceneNodeDetailMeshesExplicit(); }

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

   /// Number of rows of grid cells in the ring buffer.  The number of columns
   /// is the same number.  This is determined by the highest number requested
   /// for a level of detail.
   int ringBufferCellRowCountFull;
   /// Horizontal size of a grid cell, both for north/south and east/west.
   float cellSizeHorizontal;
   /// Coefficient applied to 'cellRowCount' for lowering the distance at
   /// which meshes are drawn.
   float cellRowCountCoefficient;
   /// Terrain on which the detail objects should be placed.
   SceneNodeTerrain* sceneNodeTerrain;
   /// Pool of cells to be populated with detail mesh instances close to the
   /// camera.
   Table<SceneNodeDetailMeshesExplicitGridCell*> ringBufferCells;
   /// Since the number of grid cells that can be refreshed on a given frame is
   /// limited, update the grid cells in an order that prioritizes those
   /// closest to the camera.  This gives an ordered list of offsets from the
   /// southwest-most cell that should be populated.
   Table<Point2I> gridCellUpdateOrder;
   /// Collection of settings for detail objects to be placed.
   Table<SceneNodeDetailMeshesExplicitDetailMeshSettings*> detailMeshesSettings;

   friend class SceneNodeDetailMeshesExplicitGridCell;
   friend class SceneNodeDetailMeshesExplicitGridCellUpdateOrderComparator;
};

//==============================================================================

class SceneNodeDetailMeshesExplicitGridCell
{
public:
   void Init(SceneNodeDetailMeshesExplicit* ownerNode);
   void Deinit();

   /// Hide any detail meshes for this cell.
   void Hide();

protected:
   /// Coordinates that were most recently used to populate this cell.
   Point2I cellCoordiantes;
   /// True if this cell is not currently populated.
   bool empty;
   /// Data for this cell which is specific to possible detail objects.
   Table<SceneNodeDetailMeshesExplicitGridCellDetailMesh*> cellDetailMeshes;

   friend class SceneNodeDetailMeshesExplicit;
};

//==============================================================================

/// Data for a given detail object, which may involve a separate representation
/// for each level of detail.
class SceneNodeDetailMeshesExplicitGridCellDetailMesh
{
public:
   SceneNodeDetailMeshesExplicitGridCellDetailMesh();

   void Init(SceneNodeDetailMeshesExplicit* ownerNode, SceneNodeDetailMeshesExplicitDetailMeshSettings* _detailMeshSettings);
   void Deinit();

   /// Hide this detail mesh.
   void Hide();

protected:
   /// Add a detail mesh for the next highest LOD.
   void LODDetailMeshAdd(SceneNode* newParent, int lodNumber, SceneNode* meshSceneRootNode);

   /// Collection of detail mesh nodes indexed by LOD.
   Table<SceneNodeMesh*> lodDetailMeshNodes;
   /// LOD level for which this cell was most recently populated.
   int lodNumber;
   /// Most recently calculated ideal LOD number for this detail object for
   /// this cell.  It is not necessarily the same as 'lodNumber'.
   int lodNumberIdeal;
   /// Settings which apply to this detail mesh.
   SceneNodeDetailMeshesExplicitDetailMeshSettings* detailMeshSettings;

   friend class SceneNodeDetailMeshesExplicit;
};

//==============================================================================

/// Data for a single instance of a detail mesh.
struct SceneNodeDetailMeshesExplicitGridCellDetailMeshInstance
{
   /// Position of the instance relative to the terrain.
   Point3F positionOverTerrain;
   /// Local rotation of the instance.
   Quaternion rotation;
   /// Local scale of the instance.
   Point3F scale;
};

//==============================================================================

/// Properties specific to a given detail object, of which there may be
/// multiple versions for different LODs.
class SceneNodeDetailMeshesExplicitDetailMeshSettings
{
public:
   SceneNodeDetailMeshesExplicitDetailMeshSettings();

   bool Init(JSONValue* specs, SceneNodeDetailMeshesExplicit* ownerNode);
   void Deinit();

protected:
   /// Filename of the scene with the detail mesh for this LOD.
   const char* sceneFilename;
   /// Preloaded instance of the scene.
   SceneInstance* preloadedSceneInstance;
   /// Settings specific to a given LOD of a given detail object.
   Table<SceneNodeDetailMeshesExplicitDetailMeshLODSettings*> lods;
   /// Array of data for all individual instances of this detail mesh, including
   /// those which are not currently visible.
   Table<SceneNodeDetailMeshesExplicitGridCellDetailMeshInstance> instances;

   friend class SceneNodeDetailMeshesExplicit;
   friend class SceneNodeDetailMeshesExplicitGridCellDetailMesh;
};

//==============================================================================

/// Properties specific to a given level-of-detail of a given detail mesh.
class SceneNodeDetailMeshesExplicitDetailMeshLODSettings
{
public:
   SceneNodeDetailMeshesExplicitDetailMeshLODSettings();

   bool Init(JSONValue* specs, SceneNodeDetailMeshesExplicit* ownerNode);
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

   friend class SceneNodeDetailMeshesExplicit;
   friend class SceneNodeDetailMeshesExplicitGridCellDetailMesh;
};

//==============================================================================

/// Functor for establishing the order in which cells should be updated.  This
/// is used instead of a static function so we can provide more context for the
/// comparison.
class SceneNodeDetailMeshesExplicitGridCellUpdateOrderComparator
{
public:
   SceneNodeDetailMeshesExplicitGridCellUpdateOrderComparator() { sceneNodeDetailMeshesExplicit = NULL; }

   bool operator()(const Point2I& a, const Point2I& b);

   /// Object which is requesting the comparisons.
   SceneNodeDetailMeshesExplicit* sceneNodeDetailMeshesExplicit;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODEDETAILMESHESEXPLICIT_H__
