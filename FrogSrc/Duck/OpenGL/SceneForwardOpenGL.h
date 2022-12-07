#ifndef __FROG__DUCK__OPENGL__SCENEFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENEFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "Duck/Scene.h"

namespace Webfoot {

class ShaderProgramGLSL;

namespace Duck {

class SceneManager;

/// Uniform declarations commonly used for fog in fragment shaders.
#define DUCK_SCENE_FORWARD_OPENGL_FRAGMENT_SHADER_FOG_VARIABLE_DECLARATIONS \
   "uniform vec3 fogColorUp;\n" \
   "uniform vec3 fogColorForward;\n" \
   "uniform vec3 fogColorDown;\n" \
   "uniform bool distanceFogEnabled;\n" \
   "uniform float distanceFogNear;\n" \
   "uniform float distanceFogRange;\n" \
   "uniform float distanceFogDensity;\n" \
   "uniform bool heightFogEnabled;\n" \
   "uniform float heightFogTop;\n" \
   "uniform float heightFogDensity;\n"

/// Function definitions commonly used for fog in fragment shaders.
#define DUCK_SCENE_FORWARD_OPENGL_FRAGMENT_SHADER_FOG_FUNCTION_DEFINITIONS \
   "vec3 FogColorGet()\n" \
   "{\n" \
   "   vec3 cameraToFragmentWorldSpace = varyingPositionWorldSpace - viewPosition;\n" \
   "   float z = cameraToFragmentWorldSpace.z / length(cameraToFragmentWorldSpace);" \
   "   return mix(mix(fogColorForward, fogColorUp, clamp(z, 0.0, 1.0)), fogColorDown, clamp(-z, 0.0, 1.0));\n" \
   "}\n" \
   "float FogInfluenceGet()\n" \
   "{\n" \
   "   float fogDistance = length(varyingPositionViewSpace);\n" \
   "   float fogInfluence = 0.0;\n" \
   "   float fogAmount = 0.0;\n" \
   "   if(distanceFogEnabled)\n" \
   "   {\n" \
   "      float distanceFromNear = max(0.0, fogDistance-distanceFogNear);\n" \
   "      float distanceFromNearOverRangeToFourth = distanceFromNear / distanceFogRange;\n" \
   "      distanceFromNearOverRangeToFourth = distanceFromNearOverRangeToFourth * distanceFromNearOverRangeToFourth;\n" \
   "      distanceFromNearOverRangeToFourth = distanceFromNearOverRangeToFourth * distanceFromNearOverRangeToFourth;\n" \
   "      fogAmount = distanceFromNear*distanceFogDensity;\n" \
   "      fogInfluence = clamp(1.0 - ((1.0 - distanceFromNearOverRangeToFourth) * exp(-fogAmount)), 0.0, 1.0);\n" \
   "   }\n" \
   "   if(heightFogEnabled)\n" \
   "   {\n" \
   "      float minHeight = min(varyingPositionWorldSpace.z, viewPosition.z);\n" \
   "      float maxHeight = max(varyingPositionWorldSpace.z, viewPosition.z);\n" \
   "      float heightRange = max(0.00001, maxHeight - minHeight);\n" \
   "      float proportionBelowHeightFogTop = min(heightRange, max(0.0, heightFogTop - minHeight)) / heightRange;\n" \
   "      fogAmount += heightFogDensity * fogDistance * proportionBelowHeightFogTop;\n" \
   "      fogInfluence = max(fogInfluence, clamp(1.0 - exp(-fogAmount), 0.0, 1.0));\n" \
   "   }\n" \
   "   return fogInfluence;\n" \
   "}\n"

/// Exponent for converting from sRGB to linear color space.
extern const float SRGB_TO_LINEAR_EXPONENT;

//==============================================================================

class SceneForwardOpenGL : public Scene
{
public:
   typedef Scene Inherited;
};

//==============================================================================

class SceneInstanceForwardOpenGL : public SceneInstance
{
public:
   SceneInstanceForwardOpenGL();

   virtual void Init(Scene* _scene);
   virtual void Draw();

   /// Set the uniforms of the given shader for fog.
   void FogUniformsSet(ShaderProgramGLSL* shaderProgram);
   /// Set the uniforms of the given shader for the lights.  The model matrix is
   /// specified explicitly in case geometry instancing fallback requires that we
   /// use the combined model matrix and instance model matrix for shadow
   /// receiving purposes.
   bool LightUniformsSet(ShaderProgramGLSL* shaderProgram, SceneInstance* sceneInstance,
      bool useLightmap, bool causticsApplicable, int* textureUnitIndex, const Matrix43& _modelMatrix);

   virtual void CascadedShadowMapsSet(JSONValue* specs);

   typedef SceneInstance Inherited;

protected:
   void SkyDraw();
   virtual void CascadedShadowMapsDeinit();
   void CascadedShadowMapsRefresh(SceneNodeLight* brightestDirectionalLight);

   #if _DEBUG
      /// Draw the given shadow map for debugging purposes.
      void DebugShadowMapDraw(GLuint shadowMapTextureID);
   #endif

   /// True if this is the first call to Draw.
   bool firstDraw;
   /// Native object for the cascaded shadow map framebuffer.
   GLuint cascadedShadowMapFramebuffer;
   /// Native object for the cascaded shadow map depth texture.
   GLuint cascadedShadowMapDepthTexture;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENEFORWARDOPENGL_H__
