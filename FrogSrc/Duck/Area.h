#ifndef __FROG__DUCK__AREA_H__
#define __FROG__DUCK__AREA_H__

#include "FrogMemory.h"
#include "Color.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

/// Folder in which Duck areas area expected.
#define DUCK_AREA_FOLDER "Duck/Areas"
/// Filename of the area specifications, excluding the extension.
#define DUCK_AREA_FILENAME "Area"
/// Pattern for building area filenames.
#define DUCK_AREA_FILENAME_PATTERN DUCK_AREA_FOLDER "/%s/" DUCK_AREA_FILENAME
/// Distance to the near clipping plane
#define DUCK_AREA_CLIP_DISTANCE_NEAR_KEY "ClipDistanceNear"
extern const float DUCK_AREA_CLIP_DISATANCE_NEAR_DEFAULT;
/// Distance to the far clipping plane
#define DUCK_AREA_CLIP_DISTANCE_FAR_KEY "ClipDistanceFar"
extern const float DUCK_AREA_CLIP_DISATANCE_FAR_DEFAULT;
/// Color of the ambient light for an area.
#define DUCK_AREA_AMBIENT_LIGHT_COLOR_KEY "AmbientLightColor"
extern const ColorRGBA8 DUCK_AREA_AMBIENT_LIGHT_COLOR_DEFAULT;
/// Multiplier for the ambient light for an area.
#define DUCK_AREA_AMBIENT_LIGHT_INTENSITY_KEY "AmbientLightIntensity"
extern const float DUCK_AREA_AMBIENT_LIGHT_INTENSITY_DEFAULT;
/// Environment map to use for the sky.
#define DUCK_AREA_SKY_ENVIRONMENT_MAP_KEY "SkyEnvironmentMap"
/// Environment map to use for the sky.
#define DUCK_AREA_SKY_ENVIRONMENT_MAP_REFLECTION_INTENSITY_KEY "SkyEnvironmentMapReflectionIntensity"
/// Color to which distance and height fog fade.
#define DUCK_AREA_FOG_COLOR_KEY "FogColor"
/// Color to which distance and height fog fade in the upward direction.
#define DUCK_AREA_FOG_COLOR_UP_KEY "FogColorUp"
/// Color to which distance and height fog fade in the forward direction.
#define DUCK_AREA_FOG_COLOR_FORWARD_KEY "FogColorForward"
/// Color to which distance and height fog fade in the downward direction.
#define DUCK_AREA_FOG_COLOR_DOWN_KEY "FogColorDown"
/// True if fog should be shown based on distance from the camera.
#define DUCK_AREA_DISTANCE_FOG_ENABLED_KEY "DistanceFogEnabled"
/// Distance at which distance fog begins.
#define DUCK_AREA_DISTANCE_FOG_NEAR_KEY "DistanceFogNear"
/// Distance at which distance fog is at full opacity.
#define DUCK_AREA_DISTANCE_FOG_FAR_KEY "DistanceFogFar"
/// True if height-based fog should be enabled.
#define DUCK_AREA_HEIGHT_FOG_ENABLED_KEY "HeightFogEnabled"
/// Height at which the density for height-based fog begins.
#define DUCK_AREA_HEIGHT_FOG_TOP_KEY "HeightFogTop"
/// Height at which height-based fog reaches full opacity when looking down
/// from the top.
#define DUCK_AREA_HEIGHT_FOG_BOTTOM_KEY "HeightFogBottom"
/// Terrain specifications for an area.
#define DUCK_AREA_TERRAIN_KEY "Terrain"
/// Which type of terrain should be created.
#define DUCK_AREA_TERRAIN_TYPE_KEY "Type"
/// Terrain type name for adpative, layered terrains.
#define DUCK_AREA_TERRAIN_TYPE_LAYERED_NAME "Layered"
/// Terrain type name for old Land-engine tiled terrains.
#define DUCK_AREA_TERRAIN_TYPE_TILED_NAME "Tiled"
/// Water specifications for an area.
#define DUCK_AREA_WATER_KEY "Water"
/// Detail mesh specifications for an area.
#define DUCK_AREA_DETAIL_MESHES_KEY "DetailMeshes"
/// Which detail mesh placement system should be used.
#define DUCK_AREA_DETAIL_MESHES_TYPE_KEY "Type"
/// Detail mesh type in which the meshes are placed explicitly.
#define DUCK_AREA_DETAIL_MESHES_TYPE_EXPLICIT_NAME "Explicit"
/// Detail mesh type in which the meshes are placed randomly.
#define DUCK_AREA_DETAIL_MESHES_TYPE_RANDOM_NAME "Random"
/// Array of entities which initially exist in the area.
#define DUCK_AREA_ENTITIES_KEY "Entities"
/// Specifications for cascaded shadow maps for the area.
#define DUCK_AREA_CASCADED_SHADOW_MAPS_KEY "CascadedShadowMaps"
/// Specifications for caustics for the area.
#define DUCK_AREA_CAUSTICS_KEY "Caustics"

/// Helper function for recursively building area specifications.
JSONValue* AreaSpecsBuildHelper(JSONValue* areasSpecs, const char* name,
   HeapID heapID, bool includeFileSpecs, JSONValue* workingSpecs);
/// Helper function for recursively building area specifications.
JSONValue* AreaSpecsBuildHelper(JSONValue* areasSpecs, JSONValue* currentLayer,
   HeapID heapID, bool includeFileSpecs, JSONValue* workingSpecs);

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__AREA_H__
