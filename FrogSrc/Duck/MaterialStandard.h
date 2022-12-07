#ifndef __FROG__DUCK__MATERIALSTANDARD_H__
#define __FROG__DUCK__MATERIALSTANDARD_H__

#include "FrogMemory.h"
#include "Duck/Material.h"

namespace Webfoot {
namespace Duck {

class MaterialInstanceStandard;

/// Material properties for the physically based metallic roughness model.
#define DUCK_MATERIAL_STANDARD_PBR_METALLIC_ROUGHNESS_KEY "PBRMetallicRoughness"
/// Base color of a standard material.
#define DUCK_MATERIAL_STANDARD_BASE_COLOR_KEY "BaseColor"
/// Texture used to determine the base color of a standard material.
#define DUCK_MATERIAL_STANDARD_BASE_COLOR_TEXTURE_KEY "BaseColorTexture"
/// Texture used to as the normal map of a standard material.
#define DUCK_MATERIAL_STANDARD_NORMAL_TEXTURE_KEY "NormalTexture"
/// Texture used to as the lightmap of a standard material.
#define DUCK_MATERIAL_STANDARD_LIGHTMAP_TEXTURE_KEY "LightmapTexture"
/// Emission color of a standard material.
#define DUCK_MATERIAL_STANDARD_EMISSION_COLOR_KEY "EmissionColor"
/// Texture for light to be emitted by a standard material.
#define DUCK_MATERIAL_STANDARD_EMISSION_TEXTURE_KEY "EmissionTexture"
/// Texture which provides the metallic, roughness, and baked ambient occlusion
/// for a standard material.
#define DUCK_MATERIAL_STANDARD_METALLIC_ROUGHNESS_OCCLUSION_TEXTURE_KEY "MetallicRoughnessOcclusionTexture"
/// Metallic value of a standard material.
#define DUCK_MATERIAL_STANDARD_METALLIC_KEY "Metallic"
/// Roughness value of a standard material.
#define DUCK_MATERIAL_STANDARD_ROUGHNESS_KEY "Roughness"
/// Base specular reflectivity for non-metals when the angle of the light is
/// perpendicular to the surface normal.  (Fresnel0 for dielectric materials)
#define DUCK_MATERIAL_STANDARD_DIELECTRIC_SPECULAR_KEY "DielectricSpecular"
/// True if both sides of the surface should be drawn.
#define DUCK_MATERIAL_STANDARD_TWO_SIDED_KEY "TwoSided"
/// For lighting purposes, true if the the normal of the back surface should
/// point in the opposite direction from that of the front surface.
#define DUCK_MATERIAL_STANDARD_BACK_FACE_NORMALS_FLIP_KEY "BackFaceNormalsFlip"
/// True if an outline should be drawn.
#define DUCK_MATERIAL_STANDARD_OUTLINE_DRAW_KEY "OutlineDraw"
/// How the alpha channel should be handled.
#define DUCK_MATERIAL_STANDARD_ALPHA_MODE_KEY "AlphaMode"
/// Threshold to use when clipping based on alpha.
#define DUCK_MATERIAL_STANDARD_ALPHA_CUTOFF_KEY "AlphaCutoff"
/// How shadows should be cast.
#define DUCK_MATERIAL_STANDARD_SHADOW_CAST_MODE_KEY "ShadowCastMode"
/// Extent to which blending should be additive.
#define DUCK_MATERIAL_STANDARD_ADDITIVE_BLENDING_KEY "AdditiveBlending"
/// True if realtime lighting should not be performed on this material.
#define DUCK_MATERIAL_STANDARD_UNLIT_KEY "Unlit"
/// True if the object should appear to scale down to zero, depending on its
/// distance from the camera.
#define DUCK_MATERIAL_STANDARD_DISTANCE_SCALE_DOWN_KEY "DistanceScaleDown"
/// When requested, this is the distance at which the object begins to scale
/// down to zero.
#define DUCK_MATERIAL_STANDARD_DISTANCE_SCALE_DOWN_NEAR_KEY "DistanceScaleDownNear"
/// When requested, this is the distance at which the object finishes scaling
/// down to zero.
#define DUCK_MATERIAL_STANDARD_DISTANCE_SCALE_DOWN_FAR_KEY "DistanceScaleDownFar"
/// True if the normals should all be treated as pointing upward in local space
/// for lighting purposes.
#define DUCK_MATERIAL_STANDARD_NORMALS_UP_LOCAL_KEY "NormalsUpLocal"
/// True if the material should move in the wind.
#define DUCK_MATERIAL_STANDARD_WIND_APPLY_KEY "WindApply"
/// Multiplied by the per-vertex data to affect how much vertices should lean
/// due to the wind.
#define DUCK_MATERIAL_STANDARD_WIND_LEAN_COEFFICIENT_KEY "WindLeanCoefficient"
/// Multiplied by the per-vertex data to affect how much vertices should shake
/// due to the wind.
#define DUCK_MATERIAL_STANDARD_WIND_SHAKE_COEFFICIENT_KEY "WindShakeCoefficient"
/// Multiplied by the per-vertex data to affect additional, probably
/// high-frequency, shaking due to the wind.
#define DUCK_MATERIAL_STANDARD_WIND_SHAKE_DETAIL_COEFFICIENT_KEY "WindShakeDetailCoefficient"

//==============================================================================

/// General-purpose material.
class MaterialStandard : public Material
{
public:
   /// Ways of interpreting the alpha channel.
   enum AlphaMode
   {
      /// Ignore alpha and draw the material at full opacity.
      ALPHA_MODE_OPAQUE,
      /// Perform normal alpha blending.
      ALPHA_MODE_BLEND,
      /// Drop any fragments with an alpha below a certain threshold.
      ALPHA_MODE_CLIP,
      /// Used with MSAA on certain platforms to provided limited
      /// order-independent transparency.
      ALPHA_MODE_ALPHA_TO_COVERAGE,
      ALPHA_MODE_COUNT
   };

   /// Ways of casting shadows.
   enum ShadowCastMode
   {
      /// Use the default shadow casting mode for the material's alpha mode.
      SHADOW_CAST_MODE_DEFAULT,
      /// Do not cast shadows.
      SHADOW_CAST_MODE_NONE,
      /// When rendering the shadow buffer, ignore alpha and treat all geometry
      /// as opaque.
      SHADOW_CAST_MODE_OPAQUE,
      /// When rendering the shadow buffer, drop any fragments with an alpha
      /// below a certain threshold. 
      SHADOW_CAST_MODE_ALPHA_CLIP,
      SHADOW_CAST_MODE_COUNT
   };

   MaterialStandard();

   virtual void Init(const char* _name, Scene* _scene, JSONValue* _specifications);
   virtual void Deinit();

   typedef Material Inherited;

   /// Return a new MaterialTexture.
   virtual MaterialTexture* MaterialTextureCreate() = 0;
   /// Return a new MaterialInstanceTexture.
   virtual MaterialInstanceTexture* MaterialInstanceTextureCreate() = 0;

protected:
   /// Base color of the material.
   ColorRGBA8 baseColor;
   /// Emission color of the material.
   ColorRGB8 emissionColor;
   /// Metallic value of the material.
   float metallic;
   /// Roughness value of the material.
   float roughness;
   /// Base specular reflectivity for non-metals when the angle of the light is
   /// perpendicular to the surface normal.  (Fresnel0 for dielectric materials)
   float dielectricSpecular;
   /// How the alpha channel should be handled.
   AlphaMode alphaMode;
   /// Threshold to use when clipping based on alpha.
   float alphaCutoff;
   /// How shadows should be cast.
   ShadowCastMode shadowCastMode;
   /// Extent to which blending should be additive.
   float additiveBlending;
   /// True if the object should appear to scale down to zero, depending on its
   /// distance from the camera.
   bool distanceScaleDown;
   /// When requested, this is the distance at which the object begins to scale
   /// down to zero.
   float distanceScaleDownNear;
   /// When requested, this is the distance at which the object finishes
   /// scaling down to zero.
   float distanceScaleDownFar;
   /// True if the normals should all be treated as pointing upward in local space
   /// for lighting purposes.
   bool normalsUpLocal;
   /// For lighting purposes, true if the the normal of the back surface should
   /// point in the opposite direction from that of the front surface.
   bool backFaceNormalsFlip;
   /// True if both sides of the surface should be drawn.
   bool twoSided;
   /// Material-specific data for the base color texture.
   MaterialTexture* baseColorMaterialTexture;
   /// Material-specific data for the lightmap.
   MaterialTexture* lightmapMaterialTexture;
   /// Material-specific data for the normal map.
   MaterialTexture* normalMaterialTexture;
   /// Material-specific data for the emission texture.
   MaterialTexture* emissionMaterialTexture;
   /// Material-specific data for the metallic/roughness/occlusion texture.
   MaterialTexture* metallicRoughnessOcclusionMaterialTexture;
   /// True if lighting should be applied when drawing the material.
   bool applyLighting;
   /// True if an outline should be drawn.
   bool outlineDraw;
   /// True if the material should move in the wind.
   bool windApply;
   /// Multiplied by the per-vertex data to affect how much vertices should
   /// lean due to the wind.
   float windLeanCoefficient;
   /// Multiplied by the per-vertex data to affect how much vertices should
   /// shake due to the wind.
   float windShakeCoefficient;
   /// Multiplied by the per-vertex data to affect additional, probably
   /// high-frequency, shaking due to the wind.
   float windShakeDetailCoefficient;
   /// Ordered list of the names of the UV maps to be used for the TexCoord
   /// channels.
   Table<const char*> uvMapNames;

   friend class MaterialInstanceStandard;
};

//==============================================================================

/// Instance of the general-purpose material.
class MaterialInstanceStandard : public MaterialInstance
{
public:
   MaterialInstanceStandard();

   virtual void Init(Material* _material);
   virtual void Deinit();

   virtual bool ShadowCasterCheck();
   virtual bool TwoSidedCheck();

   /// Set the base color of the material instance.
   void BaseColorSet(const ColorRGBA8& _baseColor) { baseColor = _baseColor; }
   /// Return the base color of the material instance.
   ColorRGBA8 BaseColorGet() { return baseColor; }
   /// Set the emmission color of the material instance.
   void EmissionColorSet(const ColorRGB8& _emissionColor) { emissionColor = _emissionColor; }
   /// Return the emmission color of the material instance.
   ColorRGB8 EmissionColorGet() { return emissionColor; }
   /// Set the threshold to use when clipping based on alpha.
   void AlphaCutoffSet(float _alphaCutoff) { alphaCutoff = _alphaCutoff; }
   /// Return the threshold to use when clipping based on alpha.
   float AlphaCutoffGet() { return alphaCutoff; }
   /// Set the extent to which blending should be additive.
   void AdditiveBlendingSet(float _additiveBlending) { additiveBlending = _additiveBlending; }
   /// Return the extent to which blending should be additive.
   float AdditiveBlendingGet() { return additiveBlending; }

   /// Return the instance-specific object for the base color texture.
   MaterialInstanceTexture* BaseColorMaterialInstanceTextureGet() { return baseColorMaterialInstanceTexture; }
   /// Return the instance-specific object for the lightmap.
   MaterialInstanceTexture* LightmapMaterialInstanceTextureGet() { return lightmapMaterialInstanceTexture; }
   /// Return the instance-specific object for the normal map.
   MaterialInstanceTexture* NormalMaterialInstanceTextureGet() { return normalMaterialInstanceTexture; }
   /// Return the instance-specific object for the emission texture.
   MaterialInstanceTexture* EmissionMaterialInstanceTextureGet() { return emissionMaterialInstanceTexture; }
   /// Return the instance-specific object for the metallic/roughness/occlusion texture.
   MaterialInstanceTexture* MetallicRoughnessOcclusionMaterialInstanceTextureGet() { return metallicRoughnessOcclusionMaterialInstanceTexture; }

   typedef MaterialInstance Inherited;

protected:
   /// Base color of the material instance.
   ColorRGBA8 baseColor;
   /// Emission color of the material instance.
   ColorRGB8 emissionColor;
   /// Threshold to use when clipping based on alpha.
   float alphaCutoff;
   /// Extent to which blending should be additive.
   float additiveBlending;
   /// Instance-specific data for the base color texture.
   MaterialInstanceTexture* baseColorMaterialInstanceTexture;
   /// Instance-specific data for the lightmap.
   MaterialInstanceTexture* lightmapMaterialInstanceTexture;
   /// Instance-specific data for the normal map.
   MaterialInstanceTexture* normalMaterialInstanceTexture;
   /// Instance-specific data for the emission texture.
   MaterialInstanceTexture* emissionMaterialInstanceTexture;
   /// Instance-specific data for the metallic/roughness/occlusion texture.
   MaterialInstanceTexture* metallicRoughnessOcclusionMaterialInstanceTexture;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__MATERIALSTANDARD_H__
