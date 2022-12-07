#ifndef __FROG__DUCK__SCENENODELIGHT_H__
#define __FROG__DUCK__SCENENODELIGHT_H__

#include "FrogMemory.h"
#include "Duck/SceneNode.h"

namespace Webfoot {
namespace Duck {

/// Type of the light.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_TYPE_KEY "LightType"
/// Color of the emitted light.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_COLOR_KEY "LightColor"
/// Energy of the emitted light.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_INTENSITY_KEY "LightIntensity"
/// Maximum distance from the light at which the light has an effect.  This
/// applies to point and spot lights.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_RANGE_KEY "LightRange"
/// Span of the outer cone of the spotlight.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_CONE_ANGLE_OUTER_KEY "LightConeAngleOuter"
/// Fraction of the cone over which to fade out.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_CONE_ANGLE_BLENDING_KEY "LightConeAngleBlending"
/// How this light should influence lightmaps.
#define DUCK_SCENE_NODE_LIGHT_LIGHT_BAKE_TYPE_KEY "LightBakeType"

//==============================================================================

/// Scene node for representing a light
class SceneNodeLight : public SceneNode
{
public:
   /// Types of light
   enum LightType
   {
      /// Sunlight
      LIGHT_TYPE_DIRECTIONAL,
      /// Omnidirectional light
      LIGHT_TYPE_POINT,
      /// Spotlight
      LIGHT_TYPE_SPOT,
      LIGHT_TYPE_COUNT
   };

   /// Ways for a light to influence lightmaps.
   enum LightBakeType
   {
      /// This light should not affect lightmaps.
      LIGHT_BAKE_TYPE_DYNAMIC,
      /// Lightmaps should include only the indirect (bounced) influence of
      /// this light.  Direct light should still be dynamic.
      LIGHT_BAKE_TYPE_INDIRECT,
      /// Lightmaps should include both direct and indirect influence of this
      /// light.
      LIGHT_BAKE_TYPE_DIRECT_AND_INDIRECT,
      LIGHT_BAKE_TYPE_COUNT
   };

   virtual void Init(const char* _name, SceneNode* _parent);
   virtual void Init(const char* _name, SceneNode* _parent, JSONValue* specifications);

   /// Return the name of the type of this node.
   virtual const char* SceneNodeTypeNameGet();

   /// Return the type of the light.
   LightType LightTypeGet() { return lightType; }
   /// Set the type of the light.
   void LightTypeSet(LightType _lightType) { lightType = _lightType; }

   /// Return the color of the emitted light.
   ColorRGB8 LightColorGet() { return lightColor; }
   /// Set the color of the emitted light.
   void LightColorSet(const ColorRGB8& _lightColor) { lightColor = _lightColor; }

   /// Return the energy of the emitted light.
   float LightIntensityGet() { return lightIntensity; }
   /// Set the energy of the emitted light.
   void LightIntensitySet(float _lightIntensity) { lightIntensity = _lightIntensity; }

   /// Return the maximum distance from the light at which the light has an
   /// effect.  This applies to point and spot lights.
   float LightRangeGet() { return lightRange; }
   /// Set the maximum distance from the light at which the light has an
   /// effect.  This applies to point and spot lights.
   void LightRangeSet(float _lightRange) { lightRange = _lightRange; }

   /// Return the span of the outer cone of the spotlight.
   float LightConeAngleOuterGet() { return lightConeAngleOuter; }
   /// Set the span of the outer cone of the spotlight.
   void LightConeAngleOuterSet(float _lightConeAngleOuter) { lightConeAngleOuter = _lightConeAngleOuter; }

   /// Return the fraction of the cone over which to fade out.
   float LightConeAngleBlendingGet() { return lightConeAngleBlending; }
   /// Set the fraction of the cone over which to fade out.
   void LightConeAngleBlendingSet(float _lightConeAngleBlending) { lightConeAngleBlending = _lightConeAngleBlending; }

   /// Return how the light should influence lightmaps.
   LightBakeType LightBakeTypeGet() { return lightBakeType; }
   /// Set how the light should influence lightmaps.
   void LightBakeTypeSet(LightBakeType _lightBakeType) { lightBakeType = _lightBakeType; }

   /// Factory method for making objects of this type.
   static SceneNode* Create() { return frog_new SceneNodeLight(); }

   typedef SceneNode Inherited;

protected:
   /// Type of the light.
   LightType lightType;
   /// Color of the emitted light.
   ColorRGB8 lightColor;
   /// Energy of the emitted light.
   float lightIntensity;
   /// Maximum distance from the light at which the light has an effect.  This
   /// applies to point and spot lights.
   float lightRange;
   /// Span of the outer cone of the spotlight.
   float lightConeAngleOuter;
   /// Fraction of the cone over which to fade out.
   float lightConeAngleBlending;
   /// How the light should influence lightmaps.
   LightBakeType lightBakeType;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__SCENENODELIGHT_H__
