#ifndef __FROG__DUCK__SCENEMANAGER_H__
#define __FROG__DUCK__SCENEMANAGER_H__

#include "FrogMemory.h"
#include "FileManager.h"
#include "FileReferenceCounter.h"
#include "Texture.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

class Animation;
class AnimationChannel;
class AnimationPlayerAnimationEvent;
class EnvironmentMap;
class Material;
class Mesh;
class Scene;
class SceneInstance;
class SceneManager;
class SceneNode;
class SceneNodeHelperSphereDrawable;
class SceneNodeLabelDrawable;
class SceneNodeMesh;
class SceneNodeProjectorTexture;
class SceneNodeSpriteDrawable;
class SceneNodeTerrainLayeredDrawable;
class SceneNodeTerrainTiledDrawable;
class SceneNodeTerrainTiledTile;
class SceneNodeWaterDrawable;

/// Type name for a general scene node.
#define DUCK_SCENE_NODE_TYPE_NAME "SceneNode"

/// Type name for a scene node for controlling the camera.
#define DUCK_SCENE_NODE_CAMERA_TYPE_NAME "Camera"
/// Type name for a explicit detail mesh scene node.
#define DUCK_SCENE_NODE_DETAIL_MESHES_EXPLICIT_TYPE_NAME "DetailMeshesExplicit"
/// Type name for a random detail mesh scene node.
#define DUCK_SCENE_NODE_DETAIL_MESHES_RANDOM_TYPE_NAME "DetailMeshesRandom"
/// Type name for a helper sphere scene node.
#define DUCK_SCENE_NODE_HELPER_SPHERE_TYPE_NAME "HelperSphere"
/// Type name for a label scene node.
#define DUCK_SCENE_NODE_LABEL_TYPE_NAME "Label"
/// Type name for a light scene node.
#define DUCK_SCENE_NODE_LIGHT_TYPE_NAME "Light"
/// Type name for a mesh scene node.
#define DUCK_SCENE_NODE_MESH_TYPE_NAME "Mesh"
/// Type name for a sprite scene node.
#define DUCK_SCENE_NODE_SPRITE_TYPE_NAME "Sprite"
/// Type name for an layered, adaptive terrain scene node.
#define DUCK_SCENE_NODE_TERRAIN_LAYERED_TYPE_NAME "TerrainLayered"
/// Type name for an tiled terrain scene node.
#define DUCK_SCENE_NODE_TERRAIN_TILED_TYPE_NAME "TerrainTiled"
/// Type name for a scene node for drawing a water surface.
#define DUCK_SCENE_NODE_WATER_TYPE_NAME "Water"

/// Type name for a standard material.
#define DUCK_MATERIAL_STANDARD_TYPE_NAME "Standard"

extern SceneManager* theScenes;

/// General ways of applying lighting to materials.
enum ShadingMode
{
   /// Simple diffuse lighting only.  No specular reflections.
   SHADING_MODE_LAMBERT,
   /// Metallic-roughness physically based rendering
   SHADING_MODE_PBR_METALLIC_ROUGHNESS
};

/// Ways of applying tone mapping.
enum ToneMappingMode
{
   /// No tone mapping is applied.
   TONE_MAPPING_MODE_NONE,
   /// Simple Reinhard tone mapping: L' = L / (1 + L)
   TONE_MAPPING_MODE_REINHARD
};

//==============================================================================

/// Collection of parameters used to initialize a SceneManager.
struct SceneManagerParameters
{
   SceneManagerParameters()
   {
      shadingModeDefault = SHADING_MODE_PBR_METALLIC_ROUGHNESS;
      toneMappingModeDefault = TONE_MAPPING_MODE_NONE;
      lightCountMax = 8;
      cascadedShadowMapsPrepare = false;
      causticsPrepare = false;
   }

   /// How materials should generally be shaded by default.
   ShadingMode shadingModeDefault;
   /// How tone mapping should be applied to scenes by default.
   ToneMappingMode toneMappingModeDefault;
   /// Maximum number of lights that can be applied to the same fragment when
   /// doing forward rendering.
   int lightCountMax;
   /// If cascading shadow maps are supported and this variable is true,
   /// systems should prepare for the possibility of cascaded shadow maps.
   bool cascadedShadowMapsPrepare;
   /// If caustics textures are supported and this variable is true,
   /// systems should prepare for the possibility of caustics.
   bool causticsPrepare;
};

//==============================================================================

class SceneManager
{
public:
   /// Create an AnimationChannel of the relevant type, but do not initialize
   /// it.
   typedef AnimationChannel* (*AnimationChannelFactory)();
   /// Create an AnimationPlayerAnimationEvent of the relevant type, but do not
   /// initialize it.
   typedef AnimationPlayerAnimationEvent* (*AnimationPlayerAnimationEventFactory)();
   /// Create a Material of the relevant type, but do not initialize it.
   typedef Material* (*MaterialFactory)();
   /// Create a SceneNode of the relevant type, but do not initialize it.
   typedef SceneNode* (*SceneNodeFactory)();

   SceneManager();

   virtual void Init(SceneManagerParameters* sceneManagerParameters);
   virtual void Deinit();

   /// Preload the scene in the given file.
   virtual Scene* Preload(const char* filename, FileManager* fileManager = theFiles);
   /// Increment the reference count of the given scene.
   virtual void Preload(Scene* scene);
   /// Undo the preloading of the scene from the given file.
   virtual void UndoPreload(const char* filename, FileManager* fileManager = theFiles);
   /// Undo the preloading of the given scene.
   virtual void UndoPreload(Scene* scene);

   /// Create an instance of the scene from the given file.
   virtual SceneInstance* Load(const char* filename, FileManager* fileManager = theFiles);
   /// Free the given scene instance.
   virtual void Unload(SceneInstance* sceneInstance);

   /// Load the animation.
   Animation* AnimationLoad(const char* filename, FileManager* fileManager = theFiles);
   /// Load the animation of the given scene.
   Animation* AnimationLoad(Scene* scene);
   /// Free the given animation.
   void AnimationUnload(Animation* animation);

   /// Load the given environment map.
   virtual EnvironmentMap* EnvironmentMapLoad(const char* filename, FileManager* fileManager = theFiles);
   /// Increment the reference count of the given environment map.
   virtual void EnvironmentMapLoad(EnvironmentMap* environmentMap);
   /// Undo the preloading of the environment map from the given file.
   virtual void EnvironmentMapUnload(const char* filename, FileManager* fileManager = theFiles);
   /// Free the given environment map.
   virtual void EnvironmentMapUnload(EnvironmentMap* environmentMap);

   /// Load the given material of the given scene file.
   virtual Material* MaterialLoad(const char* filename, const char* materialName, FileManager* fileManager = theFiles);
   /// Load the given material of the given scene.
   virtual Material* MaterialLoad(Scene* scene, const char* materialName);
   /// Free the given material.
   virtual void MaterialUnload(Material* material);

   /// Load the given mesh of the given scene file.
   virtual Mesh* MeshLoad(const char* filename, const char* meshName, FileManager* fileManager = theFiles);
   /// Load the given mesh of the given scene.
   virtual Mesh* MeshLoad(Scene* scene, const char* meshName);
   /// Free the given mesh.
   virtual void MeshUnload(Mesh* mesh);

   /// Create a new scene node based on the given specifications.  Optionally
   /// include 'parentNode' for debug output purposes.
   SceneNode* SceneNodeCreate(JSONValue* sceneNodeSpecs, SceneNode* parentNode = NULL);
   /// Unload the given scene node.
   virtual void SceneNodeUnload(SceneNode* sceneNode);

   /// Return a new EnvironmentMap.
   virtual EnvironmentMap* EnvironmentMapCreate() = 0;
   /// Return a new Mesh.
   virtual Mesh* MeshCreate() = 0;
   /// Return a new Scene.
   virtual Scene* SceneCreate() = 0;
   /// Return a new SceneInstance.
   virtual SceneInstance* SceneInstanceCreate() = 0;
   /// Return a new SceneNodeHelperSphereDrawable.
   virtual SceneNodeHelperSphereDrawable* SceneNodeHelperSphereDrawableCreate() = 0;
   /// Return a new SceneNodeLabelDrawable.
   virtual SceneNodeLabelDrawable* SceneNodeLabelDrawableCreate() = 0;
   /// Return a new SceneNodeMesh.
   virtual SceneNodeMesh* SceneNodeMeshCreate() = 0;
   /// Return a new SceneNodeProjectorTexture
   virtual SceneNodeProjectorTexture* SceneNodeProjectorTextureCreate() = 0;
   /// Return a new SceneNodeSpriteDrawable.
   virtual SceneNodeSpriteDrawable* SceneNodeSpriteDrawableCreate() = 0;
   /// Return a new SceneNodeTerrainLayeredDrawable.
   virtual SceneNodeTerrainLayeredDrawable* SceneNodeTerrainLayeredDrawableCreate() = 0;
   /// Return a new SceneNodeTerrainTiledDrawable.
   virtual SceneNodeTerrainTiledDrawable* SceneNodeTerrainTiledDrawableCreate() = 0;
   /// Return a new SceneNodeTerrainTiledTile.
   virtual SceneNodeTerrainTiledTile* SceneNodeTerrainTiledTileCreate() = 0;
   /// Return a new SceneNodeWaterDrawable.
   virtual SceneNodeWaterDrawable* SceneNodeWaterDrawableCreate() = 0;

   /// Associate the given AnimationChannel factory with the given type name.
   void AnimationChannelFactoryRegister(const char* name, AnimationChannelFactory materialFactory);
   /// Return the AnimationChannel factory with the given type name.
   AnimationChannelFactory AnimationChannelFactoryGet(const char* name);

   /// Associate the given factory with the given type name.
   void AnimationPlayerAnimationEventFactoryRegister(const char* name, AnimationPlayerAnimationEventFactory factory);
   /// Return the factory with the given type name.
   AnimationPlayerAnimationEventFactory AnimationPlayerAnimationEventFactoryGet(const char* name);

   /// Associate the given Material factory with the given type name.
   void MaterialFactoryRegister(const char* name, MaterialFactory materialFactory);
   /// Return the Material factory with the given type name.
   MaterialFactory MaterialFactoryGet(const char* name);

   /// Associate the given SceneNode factory with the given type name.
   void SceneNodeFactoryRegister(const char* name, SceneNodeFactory sceneNodeFactory);
   /// Return the SceneNode factory with the given type name.
   SceneNodeFactory SceneNodeFactoryGet(const char* name);

   /// Return how materials should generally be shaded by default.
   ShadingMode ShadingModeDefaultGet() { return shadingModeDefault; }

   /// Return how the scene should be tone mapped by default.
   ToneMappingMode ToneMappingModeDefaultGet() { return toneMappingModeDefault; }

   /// Return the maximum number of lights that can be applied to the same
   /// fragment when doing forward rendering.
   int LightCountMaxGet() { return lightCountMax; }

   /// Return true if cascaded shadow maps are supported.
   bool CascadedShadowMapsSupportedCheck() { return cascadedShadowMapsSupported; }
   /// Return true if systems should prepare for the possibility of cascaded
   /// shadow maps.
   bool CascadedShadowMapsPrepareCheck() { return cascadedShadowMapsPrepare; }

   /// Return true if systems should prepare for the possibility of caustics.
   bool CausticsPrepareCheck() { return causticsPrepare; }

protected:
   typedef Map<const char*, AnimationChannelFactory> AnimationChannelFactoryMap;
   typedef Map<const char*, AnimationPlayerAnimationEventFactory> AnimationPlayerAnimationEventFactoryMap;
   typedef Map<const char*, MaterialFactory> MaterialFactoryMap;
   typedef Map<const char*, SceneNodeFactory> SceneNodeFactoryMap;

   /// True if the system is between Init and Deinit calls.
   bool initialized;
   /// True if the ParticleManager3D should be deinitialized when this is
   /// deinitialized.
   bool deinitParticleManager3D;
   /// How materials should generally be shaded by default.
   ShadingMode shadingModeDefault;
   /// How tone mapping should be applied to scenes by default.
   ToneMappingMode toneMappingModeDefault;
   /// Maximum number of lights that can be applied to the same fragment when
   /// doing forward rendering.
   int lightCountMax;
   /// True if cascaded shadow maps are supported.
   bool cascadedShadowMapsSupported;
   /// True if systems should prepare for the possibility of cascaded shadow
   /// shadow maps.
   bool cascadedShadowMapsPrepare;
   /// True if systems should prepare for the possibility of caustics.
   bool causticsPrepare;
   /// Texture to use for caustics when none are available.
   Texture* causticsDummyTexture;
   /// Collection of scenes.
   FileReferenceCounter scenes;
   /// Collection of environment maps.
   FileReferenceCounter environmentMaps;
   /// Map of names to functions that can create the associated objects.
   AnimationChannelFactoryMap animationChannelFactories;
   /// Map of names to functions that can create the associated objects.
   AnimationPlayerAnimationEventFactoryMap animationPlayerAnimationEventFactories;
   /// Map of names to functions that can create the associated objects.
   MaterialFactoryMap materialFactories;
   /// Map of names to functions that can create the associated objects.
   SceneNodeFactoryMap sceneNodeFactories;

   friend class SceneInstance;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENEMANAGER_H__
