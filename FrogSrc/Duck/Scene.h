#ifndef __FROG__DUCK__SCENE_H__
#define __FROG__DUCK__SCENE_H__

#include "FrogMemory.h"
#include "BSONIterator.h"
#include "Frustum.h"
#include "Map.h"
#include "Matrix44.h"
#include "Sprite.h"
#include "Table.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

class Animation;
class CameraController;
class Drawable;
class EnvironmentMap;
class LensFlare;
class Material;
class Mesh;
class SceneManager;
class SceneNodeLight;
class SceneNodeProjector;

/// File extension for optimized Duck scene files.
#define DUCK_SCENE_OPTIMIZED_EXTENSION "dsc"
/// Expected version number of a Duck scene file.
#define DUCK_SCENE_VERSION_NUMBER 0
/// Part of a Duck scene file containing materials.
#define DUCK_SCENE_MATERIALS_KEY "Materials"
/// Part of a Duck scene file containing meshes.
#define DUCK_SCENE_MESHES_KEY "Meshes"
/// Part of a Duck scene file for nodes and general scene properties.
#define DUCK_SCENE_SCENE_KEY "Scene"
/// Part of the scene specifications for the root node.
#define DUCK_SCENE_ROOT_KEY "Root"
/// part of a Duck scene file for animation data.
#define DUCK_SCENE_ANIMATION_KEY "Animation"

/// Include this much space in the light frustum in the direction of the light
/// starting from the camera position.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_LIGHT_BACK_UP_DISTANCE_KEY "LightBackUpDistance"
/// True if the application of cascaded shadow map should have a limited
/// distance in addition to the limit from the view distance.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_DISTANCE_MAX_ENABLED_KEY "DistanceMaxEnabled"
/// Maximum distance at which to apply cascaded shadow maps, assuming this
/// limit is enabled.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_DISTANCE_MAX_KEY "DistanceMax"
/// Distance over which the application of cascaded shadow maps should fade
/// out, assuming the distance limit is enabled.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_DISTANCE_MAX_FADE_LENGTH_KEY "DistanceMaxFadeLength"
/// Settings for the individual slices of cascaded shadow maps.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_SLICES_KEY "Slices"
/// Constant depth offset used when determining whether something is in
/// shadow.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_SLICE_BIAS_KEY "Bias"
/// Multiplied by the slope of the shadow-receiving surface and subtracted
/// from depth when determining whether something is in shadow.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_SLICE_NORMAL_BIAS_KEY "NormalBias"
/// Settings for the separations between cascaded shadow map slices.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_SPLITS_KEY "Splits"
/// Fraction of the distance from the camera's near plane to the maximum
/// distance for shadows at which this separation should be made, excluding
/// the effect of blending between slices.  This maximum distance may be
/// either explicit or the distance to the far plane of the camera.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_SPLIT_DEPTH_COEFFICIENT_KEY "DepthCoefficient"
/// Fraction of the slice before the split over which to ease in the
/// influence of the slice after the split.
#define DUCK_SCENE_CASCADED_SHADOW_MAPS_SPLIT_BLEND_LENGTH_COEFFICIENT_KEY "BlendLengthCoefficient"

/// Sprite resource file for providing a caustics sprite animation.
#define DUCK_SCENE_CAUSTICS_SPRITE_RESOURCE_FILENAME_KEY "SpriteResourceFilename"
/// Sprite animation to apply to the main light for caustics.
#define DUCK_SCENE_CAUSTICS_SPRITE_ANIMATION_KEY "SpriteAnimation"
/// Width and height of the caustics pattern in world units when applied.
#define DUCK_SCENE_CAUSTICS_TEXTURE_SIZE_WORLD_SPACE_KEY "TextureSizeWorldSpace"
/// World-space height at which the influence of caustics begins to fade in.
#define DUCK_SCENE_CAUSTICS_TOP_HEIGHT_KEY "TopHeight"
/// Vertical distance over which the influence of caustics reaches its maximum.
#define DUCK_SCENE_CAUSTICS_TOP_FADE_DISTANCE_KEY "TopFadeDistance"
/// Distance from the top height of the caustics at which light attenuation
/// begins.
#define DUCK_SCENE_CAUSTICS_ATTENUATION_DISTANCE_BEGIN_KEY "AttenuationDistanceBegin"
/// Distance from the top height of the caustics at which light is completely
/// attenuated.
#define DUCK_SCENE_CAUSTICS_ATTENUATION_DISTANCE_END_KEY "AttenuationDistanceEnd"

//==============================================================================

/// Data for a scene shared across all instances of the scene for a given
/// SceneManager.
class Scene
{
public:
   Scene();

   /// Initialize the scene from the given specifications.  Remember the given
   /// filename and FileManager as the source from which this was loaded.  This
   /// provides context for files referenced within the scene.
   virtual void Init(BSONIterator fileSpecs, const char* _filename, FileManager* _fileManager);
   virtual void Deinit();

   /// Return specifications for the scene and its nodes.
   JSONValue* SceneSpecsGet() { return sceneSpecs; }

   /// Return the filename used to load this, if any.
   const char* FilenameGet() { return filename; }
   /// Return the FileManager from which this was loaded, if any.
   FileManager* FileManagerGet() { return fileManager; }

   /// Return the animation data for the scene, if any.
   Animation* AnimationGet() { return animation; }
   /// Return the material with the given name.
   Material* MaterialGet(const char* _name);
   /// Return the mesh with the given name.
   Mesh* MeshGet(const char* _name);

protected:
   typedef Map<const char*, Material*> MaterialMap;
   typedef Map<const char*, Mesh*> MeshMap;

   /// True if between calls to Init and Deinit.
   bool initialized;
   /// Specifications for the scene and its nodes.
   JSONValue* sceneSpecs;
   /// Animation data, if any.
   Animation* animation;
   /// Filename used to load this, if any.
   const char* filename;
   /// FileManager from which this was loaded, if any.
   FileManager* fileManager;
   /// Materials in this scene.
   MaterialMap materials;
   /// Meshes in this scene.
   MeshMap meshes;
};

//==============================================================================

/// Data for a frustum section in cascaded shadow maps.
struct SceneInstanceCascadedShadowMapsSlice
{
   /// View matrix to be used for the light for this slice.
   Matrix43 lightViewMatrix;
   /// Projection matrix to be used for the light for this slice.
   Matrix44 lightProjectionMatrix;
   /// View-Projection matrix to be used for the light for this slice.
   Matrix44 lightViewProjectionMatrix;
   /// Constant depth offset used when determining whether something is in
   /// shadow.
   float bias;
   /// Multiplied by the slope of the shadow-receiving surface and subtracted
   /// from depth when determining whether something is in shadow.
   float normalBias;
};

//==============================================================================

/// Data for a separation between frustum sections in cascaded shadow maps.
struct SceneInstanceCascadedShadowMapsSplit
{
   /// Fraction of the distance from the camera's near plane to the maximum
   /// distance for shadows at which this separation should be made, excluding
   /// the effect of blending between slices.  This maximum distance may be
   /// either explicit or the distance to the far plane of the camera.
   float depthCoefficient;
   /// Distance in world units from the camera position to this separation.
   float depth;
   /// Fraction of the slice before the split over which to ease in the
   /// influence of the slice after the split.
   float blendLengthCoefficient;
   /// Length in world units before the split over which to ease in the
   /// influence of the slice after the split.
   float blendLength;
};

//==============================================================================

/// Instance of a Scene.
class SceneInstance
{
public:
   SceneInstance();
   virtual ~SceneInstance() {}

   virtual void Init(Scene* _scene);
   virtual void Deinit();
   virtual void Update(unsigned int dt);
   virtual void Draw() = 0;

   /// Return the root node of this scene.
   SceneNode* RootSceneNodeGet() { return rootSceneNode; }
   /// Detach the root scene node from this scene instance.
   void RootSceneNodeDetach();

   /// Set the matrix for going from view space to clip space.
   void ProjectionMatrixSet(const Matrix44& _projectionMatrix) { projectionMatrix = _projectionMatrix; }
   /// Return the matrix for going from view space to clip space.
   Matrix44 ProjectionMatrixGet() { return projectionMatrix; }
   /// Set the matrix for going from world space to view space.
   void ViewMatrixSet(const Matrix43& _viewMatrix) { viewMatrix = _viewMatrix; }
   /// Return the matrix for going from world space to view space.
   Matrix43 ViewMatrixGet() { return viewMatrix; }
   /// Set the view matrix excluding any offset added for stereoscopy.
   void ViewMatrixCenterSet(const Matrix43& _viewMatrixCenter) { viewMatrixCenter = _viewMatrixCenter; }
   /// Return the view matrix excluding any offset added for stereoscopy.
   Matrix43 ViewMatrixCenterGet() { return viewMatrixCenter; }
   /// Set the matrix for going from the local space of the camera (not to be
   /// confused with view space) into world space.
   void CameraTransformSet(const Matrix43& _cameraTransform) { cameraTransform = _cameraTransform; }
   /// Return the matrix for going from the local space of the camera (not to
   /// be confused with view space) into world space.
   Matrix43 CameraTransformGet() { return cameraTransform; }
   /// Same as cameraTransform, but excluding any offset added for
   /// stereoscopy.
   void CameraTransformCenterSet(const Matrix43& _cameraTransformCenter) { cameraTransformCenter = _cameraTransformCenter; }
   /// Same as cameraTransform, but excluding any offset added for
   /// stereoscopy.
   Matrix43 CameraTransformCenterGet() { return cameraTransformCenter; }
   /// Set the matrix for going from local space to world space.
   void ModelMatrixSet(const Matrix43& _modelMatrix) { modelMatrix = _modelMatrix; }
   /// Return the matrix for going from local space to world space.
   Matrix43 ModelMatrixGet() { return modelMatrix; }

   /// Set the frustum used for view frustum culling.
   void ViewFrustumSet(const Frustum& _viewFrustum) { viewFrustum = _viewFrustum; }
   /// Return the frustum used for view frustum culling.
   Frustum ViewFrustumGet() { return viewFrustum; }

   /// Given a position in world space, set 'screenSpacePosition' to its
   /// position on the screen.  Return true if successful.  Return false if the
   /// point would be clipped.  Optionally specify which planes should be
   /// applied for clipping using Frustum::Side flags.
   bool WorldSpaceToScreenSpace(Point2F* screenSpacePosition,
      const Point3F& worldSpacePosition, int clippingPlanes = Frustum::SIDES_ALL);
   /// Return a ray pointing into the world based on the current center camera
   /// transform, projection matrix, and the given screen position.
   Ray3 ScreenPositionRayGet(const Point2F& screenPosition);

   /// Set the bounding sphere of the current drawable in local space.  This
   /// should include the influence of bounding volume scaling.
   void DrawableBoundingSphereSet(const Sphere& _drawableBoundingSphere) { drawableBoundingSphere = _drawableBoundingSphere; }
   /// Return the bounding sphere of the current drawable in local space.  This
   /// should include the influence of bounding volume scaling.
   Sphere DrawableBoundingSphereGet() { return drawableBoundingSphere; }

   /// Set the CameraController, if any.
   void CameraControllerSet(CameraController* _cameraController) { cameraController = _cameraController; }
   /// Return the current CameraController, if any.
   CameraController* CameraControllerGet() { return cameraController; }
   /// Set whether cameras should be responding to user input.
   void CameraControllersAcceptingInputSet(bool _cameraControllersAcceptingInput) { cameraControllersAcceptingInput = _cameraControllersAcceptingInput; }
   /// Return true if cameras should be responding to user input.
   bool CameraControllersAcceptingInputCheck() { return cameraControllersAcceptingInput; }

   /// Set the color to use for the ambient light.
   void AmbientLightColorSet(const ColorRGB8& _ambientLightColor) { ambientLightColor = _ambientLightColor; }
   /// Return the color to use for the ambient light.
   ColorRGB8 AmbientLightColorGet() { return ambientLightColor; }
   /// Set the intensity of the ambient light.
   void AmbientLightIntensitySet(float _ambientLightIntensity) { ambientLightIntensity = _ambientLightIntensity; }
   /// Return the intensity of the ambient light.
   float AmbientLightIntensityGet() { return ambientLightIntensity; }

   /// Set the environment map to use for the sky.
   void SkyEnvironmentMapSet(const char* filename, FileManager* fileManager = theFiles);
   /// Set the environment map to use for the sky.
   void SkyEnvironmentMapSet(EnvironmentMap* _skyEnvironmentMap);
   /// Return the environment map to use for the sky, if any.
   EnvironmentMap* SkyEnvironmentMapGet() { return skyEnvironmentMap; }
   /// Set the color to be applied to the sky environment map.
   void SkyEnvironmentMapColorSet(const ColorRGB8& _skyEnvironmentMapColor) { skyEnvironmentMapColor = _skyEnvironmentMapColor; }
   /// Return the color to be applied to the sky environment map.
   ColorRGB8 SkyEnvironmentMapColorGet() { return skyEnvironmentMapColor; }
   /// Set the multiplier for reflections of the sky environment map.
   void SkyEnvironmentMapReflectionIntensitySet(float _skyEnvironmentMapReflectionIntensity) { skyEnvironmentMapReflectionIntensity = _skyEnvironmentMapReflectionIntensity; }
   /// Return the multiplier for reflections of the sky environment map.
   float SkyEnvironmentMapReflectionIntensityGet() { return skyEnvironmentMapReflectionIntensity; }

   /// Set the color to which distance and height fog fade.
   void FogColorSet(const ColorRGB8& _fogColor);
   /// Set the gradient of colors to which distance and height fog fade.
   void FogColorGradientSet(const ColorRGB8& _fogColorUp, const ColorRGB8& _fogColorForward, const ColorRGB8& _fogColorDown);
   /// Return the color to which distance and height fog fade.
   ColorRGB8 FogColorGet() { return fogColorForward; }
   /// Return the color to which distance and height fog fade in the upward
   /// direction.
   ColorRGB8 FogColorUpGet() { return fogColorUp; }
   /// Return the color to which distance and height fog fade in the forward
   /// direction.
   ColorRGB8 FogColorForwardGet() { return fogColorForward; }
   /// Return the color to which distance and height fog fade in the downward
   /// direction.
   ColorRGB8 FogColorDownGet() { return fogColorDown; }
   /// Set whether fog should be shown based on distance from the camera.
   void DistanceFogEnabledSet(bool _distanceFogEnabled) { distanceFogEnabled = _distanceFogEnabled; }
   /// Return true if fog should be shown based on distance from the camera.
   bool DistanceFogEnabledCheck() { return distanceFogEnabled; }
   /// Set the distance at which distance fog begins.
   void DistanceFogNearSet(float _distanceFogNear) { distanceFogNear = _distanceFogNear; }
   /// Return the distance at which distance fog begins.
   float DistanceFogNearGet() { return distanceFogNear; }
   /// Set the distance at which distance fog becomes fully opaque.
   void DistanceFogFarSet(float _distanceFogFar) { distanceFogFar = _distanceFogFar; }
   /// Return the distance at which distance fog begins.
   float DistanceFogFarGet() { return distanceFogFar; }
   /// Set the variable for controlling the density of fog based on distance
   /// from the camera.
   void DistanceFogDensitySet(float _distanceFogDensity) { distanceFogDensity = _distanceFogDensity; }
   /// Return the variable for controlling the density of fog based on distance
   /// from the camera.
   float DistanceFogDensityGet() { return distanceFogDensity; }
   /// Based on the current near and far distances for distance fog, compute a
   /// fog density.
   void DistanceFogDensityCompute();
   /// Set whether fog should be shown based on height.
   void HeightFogEnabledSet(bool _heightFogEnabled) { heightFogEnabled = _heightFogEnabled; }
   /// Return true if fog should be shown based on height.
   bool HeightFogEnabledCheck() { return heightFogEnabled; }
   /// Set the threshold at which height-based fog begins.
   void HeightFogTopSet(float _heightFogTop) { heightFogTop = _heightFogTop; }
   /// Return the threshold at which height-based fog begins.
   float HeightFogTopGet() { return heightFogTop; }
   /// Set the height at which height-based fog reaches full opacity when
   /// looking down from the top.
   void HeightFogBottomSet(float _heightFogBottom) { heightFogBottom = _heightFogBottom; }
   /// Return the height at which height-based fog reaches full opacity when
   /// looking down from the top.
   float HeightFogBottomGet() { return heightFogBottom; }
   /// Set the variable for controllng the density of fog below the height
   /// threshold.
   void HeightFogDensitySet(float _heightFogDensity) { heightFogDensity = _heightFogDensity; }
   /// Return the variable for controlling the density of fog below the height
   /// threshold.
   float HeightFogDensityGet() { return heightFogDensity; }
   /// Based on the current top and bottom heights for height fog, compute a
   /// fog density.
   void HeightFogDensityCompute();

   /// Set up lens flare according to the given specifications.
   void LensFlareSet(JSONValue* specs);
   /// Set whether lens flare should be shown.
   void LensFlareVisibleSet(bool _lensFlareVisible) { lensFlareVisible = _lensFlareVisible; }
   /// Return true if lens flare should be shown.
   bool LensFlareVisibleCheck() { return lensFlareVisible; }

   /// Set the maximum thickness used to draw outlines in logical pixels.
   void OutlineThicknessMaxSet(float _outlineThicknessMax) { outlineThicknessMax = _outlineThicknessMax; }
   /// Return the maximum thickness used to draw outlines in logical pixels.
   float OutlineThicknessMaxGet() { return outlineThicknessMax; }

   /// Refresh the text for all the nodes.  This is called automatically when
   /// the language changes.
   void TextRefresh();

   /// Return the scene, if any, on which this instance is based.
   Scene* SceneGet() { return scene; }

   /// Return the temporary collection of lights.  This list may only be
   /// populated while drawing the scene.
   Table<SceneNodeLight*>* LightsGet() { return &lights; }
   /// Return the temporary collection of Drawable objects under consideration
   /// at the moment.  This list may only be populatd while drawing the scene
   /// and may not include all drawables in the scene at any given time.  This
   /// is for internal use.
   Table<Drawable*>* DrawablesGet() { return &drawables; }

   /// By default, if no lights are added to the scene, a set of default lights
   /// become visible.  To leave the default lights invisible, even when there
   /// are no other lights, call this with 'false'.  This should be set before
   /// the first time the scene is drawn, if at all.
   void DefaultLightsAllowedSet(bool _defaultLightsAllowed) { defaultLightsAllowed = _defaultLightsAllowed; }
   /// See 'DefaultLightsAllowedSet'.
   bool DefaultLightsAllowedCheck() { return defaultLightsAllowed; }

   /// Prepare for cascaded shadow maps.
   virtual void CascadedShadowMapsSet(JSONValue* specs);
   /// Return true if cascaded shadow maps should be used.
   bool CascadedShadowMapsEnabledCheck() { return cascadedShadowMapsEnabled; }
   /// Set whether cascaded shadow maps should be used.
   void CascadedShadowMapsEnabledSet(bool _cascadedShadowMapsEnabled) { cascadedShadowMapsEnabled = _cascadedShadowMapsEnabled; }

   /// Prepare for caustics.
   void CausticsSet(JSONValue* specs);
   /// Return true if caustics should be used.
   bool CausticsEnabledCheck() { return causticsEnabled; }
   /// Set whether caustics should be used.
   void CausticsEnabledSet(bool _causticsEnabled) { causticsEnabled = _causticsEnabled; }
   /// Return the height at which caustics begin.
   float CausticsTopHeightGet() { return causticsTopHeight; }
   /// Set the height at which caustics begin.
   void CausticsTopHeightSet(float _causticsTopHeight) { causticsTopHeight = _causticsTopHeight; }

   /// Return the value used to animate the wind.
   float WindTimeGet() { return windTime; }
   /// Set the direction of the wind using an angle in degrees.  0 degrees
   /// corresponds to +y, 90 degrees corresponds to -x, and so on.
   void WindAngleSet(float _windAngle) { windAngle = _windAngle; }
   /// See 'WindAngleGet'
   float WindAngleGet() { return windAngle; }
   /// Set the range for the magnitude of the wind.
   void WindMagnitudeSet(float _windMagnitudeMin, float _windMagnitudeMax) { windMagnitudeMin = _windMagnitudeMin; windMagnitudeMax = _windMagnitudeMax; }
   /// Return the minimum magnitude of the wind.
   float WindMagnitudeMinGet() { return windMagnitudeMin; }
   /// Return the maximum magnitude of the wind.
   float WindMagnitudeMaxGet() { return windMagnitudeMax; }

   /// Set the viewport to which this scene should draw.  This should be given
   /// in screen coordinates from the game's perspective, not necessarily that
   /// of the underlying framebuffer.
   void ViewportSet(const Box2F& _viewport) { viewport = _viewport; }
   /// See 'ViewportSet'.
   Box2F ViewportGet() { return viewport; }

   /// Set whether the scene should ideally be drawn in wireframe.  This is not
   /// necessarily supported with all materials, renderers, or platforms, but
   /// may still be useful for tools.
   void WireframeSet(bool _wireframe) { wireframe = _wireframe; }
   /// Return true if the scene should ideally be drawn in wireframe.  This is
   /// not necessarily supported with all materials, renderers, or platforms,
   /// but may still be useful for tools.
   bool WireframeCheck() { return wireframe; }

protected:
   enum
   {
      /// Number of slices in cascaded shadow maps.
      CASCADED_SHADOW_MAPS_SLICE_COUNT = 4
   };

   /// Gather the latest details from the camera controller.
   void CameraRefresh();
   /// Helper function for recursively caching scene node data which may
   /// otherwise need to be recomputed redundantly.
   void SceneNodesDrawPrepare(SceneNode* node);
   /// Helper function for building collections of drawables.
   void DrawablesGather(Table<Drawable*>* drawables, Frustum* cullingFrustum,
      SceneNode* node, SceneNode::DrawablesGatherMode drawablesGatherMode);
   /// Helper function for building collections of things like lights and
   /// projectors.
   void SpecialNodesGather(SceneNode* node);
   /// Set the visibility of the default lights based on whether there are
   /// currently any other lights in the scene.
   void DefaultLightsRefresh();
   /// Clean up cascaded shadow maps.
   virtual void CascadedShadowMapsDeinit();
   /// Prepare the cascaded shadow maps for this frame.
   void CascadedShadowMapsRefresh(SceneNodeLight* brightestDirectionalLight);
   /// Clean up caustics.
   void CausticsDeinit();
   /// Prepare to apply caustics for this frame.
   void CausticsRefresh(SceneNodeLight* brightestDirectionalLight);

   /// Comparator for determining the priority of lights when there is a limit on
   /// the number of lights which are applied to a material.
   static bool LightPriorityComparator(SceneNodeLight* lightA, SceneNodeLight* lightB);

   /// Scene, if any, upon which this instance is based.
   Scene* scene;
   /// Root node of this scene instance.
   SceneNode* rootSceneNode;
   /// Object which currently manages the view matrix.
   CameraController* cameraController;
   /// True if cameras should be responding to user input.
   bool cameraControllersAcceptingInput;
   /// Viewport to which this scene should draw.  This should be given in
   /// screen coordinates from the game's perspective, not necessarily that of
   /// the underlying framebuffer.
   Box2F viewport;
   /// Color to use for the ambient light.
   ColorRGB8 ambientLightColor;
   /// Intensity of the ambient light.
   float ambientLightIntensity;
   /// Matrix for going from view space to clip space.
   Matrix44 projectionMatrix;
   /// Matrix for going from world space to view space.
   Matrix43 viewMatrix;
   /// View matrix excluding any offset added for stereoscopy.
   Matrix43 viewMatrixCenter;
   /// Matrix for going from the local space of the camera (not to be confused
   /// with view space) into world space.
   Matrix43 cameraTransform;
   /// Same as cameraTransform, but excluding any offset added for
   /// stereoscopy.
   Matrix43 cameraTransformCenter;
   /// Matrix for going from model space to world space.
   Matrix43 modelMatrix;
   /// Bounding sphere of the current drawable in local space.
   Sphere drawableBoundingSphere;
   /// Used for view frustum culling.
   Frustum viewFrustum;
   /// EnvironmentMap to use for the sky.
   EnvironmentMap* skyEnvironmentMap;
   /// True if 'skyEnvironmentMap' should be unloaded when it is no longer
   /// needed.
   bool shouldUnloadSkyEnvironmentMap;
   /// Color to apply to the sky environment map.
   ColorRGB8 skyEnvironmentMapColor;
   /// Multiplier for reflections of the sky environment map.
   float skyEnvironmentMapReflectionIntensity;
   /// Color to which distance fog fades in the upward direction.
   ColorRGB8 fogColorUp;
   /// Color to which distance fog fades in the forward direction.
   ColorRGB8 fogColorForward;
   /// Color to which distance fog fades in the downward direction.
   ColorRGB8 fogColorDown;
   /// True if fog should be shown based on distance from the camera.
   bool distanceFogEnabled;
   /// Distance at which distance fog begins.
   float distanceFogNear;
   /// Distance at which distance fog is at full opacity.
   float distanceFogFar;
   /// Variable for controlling the density of fog based on distance from the
   /// camera.
   float distanceFogDensity;
   /// True if fog should be shown based on height.
   bool heightFogEnabled;
   /// Threshold at which height-based fog begins.
   float heightFogTop;
   /// Height at which height-based fog reaches full opacity when looking down
   /// from the top.
   float heightFogBottom;
   /// Variable for controlling the density of fog below the height threshold.
   float heightFogDensity;
   /// True if the scene should ideally be drawn in wireframe.  This is not
   /// necessarily supported with all materials, renderers, or platforms, but
   /// may still be useful for tools.
   bool wireframe;
   /// See 'DefaultLightsAllowedSet'.
   bool defaultLightsAllowed;
   /// Object for managing lens flares.
   LensFlare* lensFlare;
   /// True if lens flare should be shown.
   bool lensFlareVisible;
   /// Maximum thickness used to draw outlines in logical pixels.
   float outlineThicknessMax;
   /// Collection for temporary groups of Drawable objects.
   Table<Drawable*> drawables;
   /// Temporary collection of lights.
   Table<SceneNodeLight*> lights;
   /// Temporary collection of projectors.
   Table<SceneNodeProjector*> projectors;
   /// Default lights to be used when there are no lights in the scene.
   Table<SceneNodeLight*> defaultLights;
   /// True if cascaded shadow maps should be used.
   bool cascadedShadowMapsEnabled;
   /// Dimensions of the depth texture used for cascaded shadow maps.
   Point2I cascadedShadowMapsTextureSize;
   /// Include this much space in the light frustum in the direction of the light
   /// starting from the camera position.
   float cascadedShadowMapsLightBackUpDistance;
   /// True if the application of cascaded shadow map should have a limited
   /// distance in addition to the limit from the view distance.
   bool cascadedShadowMapsDistanceMaxEnabled;
   /// Maximum distance at which to apply cascaded shadow maps, assuming this
   /// limit is enabled.
   float cascadedShadowMapsDistanceMax;
   /// Distance over which the application of cascaded shadow maps should fade
   /// out, assuming the distance limit is enabled.
   float cascadedShadowMapsDistanceMaxFadeLength;
   /// Data for each frustum section in cascaded shadow maps.
   SceneInstanceCascadedShadowMapsSlice cascadedShadowMapsSlices[CASCADED_SHADOW_MAPS_SLICE_COUNT];
   /// Data for separations between frustum sections in cascaded shadow maps.
   SceneInstanceCascadedShadowMapsSplit cascadedShadowMapsSplits[CASCADED_SHADOW_MAPS_SLICE_COUNT-1];
   /// True if caustics textures should be used.
   bool causticsEnabled;
   /// World-space height at which the influence of caustics begins to fade in.
   float causticsTopHeight;
   /// Vertical distance over which the influence of caustics reaches its
   /// maximum.
   float causticsTopFadeDistance;
   /// Distance from the top height of the caustics at which light attenuation
   /// begins.
   float causticsAttenuationDistanceBegin;
   /// Distance from the top height of the caustics at which light is completely
   /// attenuated.
   float causticsAttenuationDistanceEnd;
   /// Computed from 'causticsAttenuationDistanceBegin' and
   /// 'causticsAttenuationDistanceEnd' in a manner similar to fog density.
   float causticsAttenuationDensity;
   /// Sprite for the caustics pattern to be applied to the main directional
   /// light;
   Sprite* causticsSprite;
   /// Current texture to use for caustics.
   Texture* causticsTexture;
   /// Width and height of the caustics pattern in world units when applied.
   Point2F causticsTextureSizeWorldSpace;
   /// Matrix for going from world space to caustics texture space.
   Matrix44 causticsViewProjectionMatrix;
   /// Time used for animating the wind.
   float windTime;
   /// Direction of the wind as an angle in degrees.  0 degrees corresponds to
   /// +y, 90 degrees corresponds to -x, and so on.
   float windAngle;
   /// Minimum magnitude of the wind.
   float windMagnitudeMin;
   /// Maximum magnitude of the wind.
   float windMagnitudeMax;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENE_H__
