#ifndef __FROG__DUCK__OPENGL__SCENEMANAGERFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__SCENEMANAGERFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "ShaderProgramGLSL.h"
#include "Duck/SceneManager.h"

namespace Webfoot {
namespace Duck {

/// Name of the shared shader uniform for the model matrix.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_MODEL_MATRIX_UNIFORM_NAME "modelMatrix"
/// Name of the shared shader uniform for the view matrix.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_VIEW_MATRIX_UNIFORM_NAME "viewMatrix"
/// Name of the shared shader uniform for the projection matrix.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_PROJECTION_MATRIX_UNIFORM_NAME "projectionMatrix"

/// Sprintf pattern for names of the shader uniforms for the types of different
/// lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_TYPE_UNIFORM_PATTERN "lightTypes%d"
/// Sprintf pattern for names of the shader uniforms for the colors of
/// different lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_COLOR_UNIFORM_PATTERN "lightColors%d"
/// Sprintf pattern for names of the shader uniforms for the directions of
/// different lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_DIRECTION_UNIFORM_PATTERN "lightDirections%d"
/// Sprintf pattern for names of the shader uniforms for the positions of
/// different lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_POSITION_UNIFORM_PATTERN "lightPositions%d"
/// Sprintf pattern for names of the shader uniforms for the cutoff distances
/// of different lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_RANGE_UNIFORM_PATTERN "lightRanges%d"
/// Sprintf pattern for names of the shader uniforms for the cosine of half the
/// inner angles of different lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_COS_HALF_INNER_ANGLE_UNIFORM_PATTERN "lightCosHalfInnerAngles%d"
/// Sprintf pattern for names of the shader uniforms for the cosine of half the
/// outer angles of different lights.
#define DUCK_SCENE_MANAGER_FORWARD_OPENGL_LIGHT_COS_HALF_OUTER_ANGLE_UNIFORM_PATTERN "lightCosHalfOuterAngles%d"

/// Collection of the names to use for the shader uniforms for the types
/// of different lights.
extern const char** LIGHT_TYPE_UNIFORMS;
/// Collection of the names to use for the shader uniforms for the colors
/// of different lights.
extern const char** LIGHT_COLOR_UNIFORMS;
/// Collection of the names to use for the shader uniforms for the directions
/// of different lights.
extern const char** LIGHT_DIRECTION_UNIFORMS;
/// Collection of the names to use for the shader uniforms for the positions
/// of different lights.
extern const char** LIGHT_POSITION_UNIFORMS;
/// Collection of the names to use for the shader uniforms for the cutoff
/// distances of different lights.
extern const char** LIGHT_RANGE_UNIFORMS;
/// Collection of the names to use for the shader uniforms for the cosine of
/// half the inner angles of different spotlights.
extern const char** LIGHT_COS_HALF_INNER_ANGLE_UNIFORMS;
/// Collection of the names to use for the shader uniforms for the cosine of
/// half the outer angles of different spotlights.
extern const char** LIGHT_COS_HALF_OUTER_ANGLE_UNIFORMS;

//==============================================================================

class SceneManagerForwardOpenGL : public SceneManager
{
public:
   SceneManagerForwardOpenGL();

   virtual void Init(SceneManagerParameters* sceneManagerParameters);
   virtual void Deinit();

   virtual EnvironmentMap* EnvironmentMapCreate();
   virtual Mesh* MeshCreate();
   virtual Scene* SceneCreate();
   virtual SceneInstance* SceneInstanceCreate();
   virtual SceneNodeHelperSphereDrawable* SceneNodeHelperSphereDrawableCreate();
   virtual SceneNodeLabelDrawable* SceneNodeLabelDrawableCreate();
   virtual SceneNodeMesh* SceneNodeMeshCreate();
   virtual SceneNodeProjectorTexture* SceneNodeProjectorTextureCreate();
   virtual SceneNodeSpriteDrawable* SceneNodeSpriteDrawableCreate();
   virtual SceneNodeTerrainLayeredDrawable* SceneNodeTerrainLayeredDrawableCreate();
   virtual SceneNodeTerrainTiledDrawable* SceneNodeTerrainTiledDrawableCreate();
   virtual SceneNodeTerrainTiledTile* SceneNodeTerrainTiledTileCreate();
   virtual SceneNodeWaterDrawable* SceneNodeWaterDrawableCreate();

   /// Return the shader program used to draw the sky.
   ShaderProgramGLSL* SkyShaderProgramGet() { return &skyShaderProgram; }

   /// Return the shader for drawing only the depth of an opaque surface.
   ShaderProgramGLSL* DepthOpaqueShaderProgramGet() { return &depthOpaqueShaderProgram; }

   /// Prepare for textured 2D Frog drawing in the 3D scene.
   void PreTextured2DDrawing(SceneInstance* sceneInstance, SceneNode* sceneNode);
   /// Clean up after a call to PreTextured2DDrawing.
   void PostTextured2DDrawing(ShaderProgramGLSL* frogTexturedShader);

   /// Prepare for untextured 2D Frog drawing in the 3D scene.
   void PreUntextured2DDrawing(SceneInstance* sceneInstance, SceneNode* sceneNode);
   /// Clean up after a call to PreUntextured2DDrawing.
   void PostUntextured2DDrawing(ShaderProgramGLSL* frogTexturedShader);

   /// Add light variables and functions to the vertex and fragment shaders. 
   /// Return true if successful.
   bool LightDeclarationsAdd(ShaderProgramGLSL* shaderProgram, bool alphaModeBlend,
      ShadingMode shadingMode, bool causticsApplicable);

   #if _DEBUG
      /// Return the debug shader for displaying shadow maps.
      ShaderProgramGLSL* DebugShadowMapShaderProgramGet() { return &debugShadowMapShaderProgram; }
   #endif

   typedef SceneManager Inherited;

protected:
   /// Prepare the shader for drawing the sky.
   void SkyShaderInit();
   /// Prepare the shader for textured Frog drawing.
   void TexturedShaderInit();
   /// Prepare the shader for untextured Frog drawing.
   void UntexturedShaderInit();
   /// Prepare the shader for drawing only the depth of an opaque surface.
   void DepthOpaqueShaderInit();
   /// Helper for preparing an array of light uniform names.
   const char** LightUniformNamesInit(const char* uniformNamePattern);
   /// Helper for cleaning up an array of light uniform names.
   void LightUniformNamesDeinit(const char**& uniformNameArray);

   #if _DEBUG
      /// Initialize the debug shader for displaying shadow maps.
      void DebugShadowMapShaderProgramInit();
   #endif

   /// Environment map to provide to shaders when none is explicitly specified.
   EnvironmentMap* environmentMapDefault;
   /// OpenGL buffer object for the shared vertex buffer.
   GLuint vertexBufferID;
   /// Shader program used to draw the sky.
   ShaderProgramGLSL skyShaderProgram;
   /// Shader program used for textured Frog drawing for things like sprites
   /// and fonts.
   ShaderProgramGLSL texturedShaderProgram;
   /// Shader program used for textured Frog drawing for things like lines
   /// and rectangles.
   ShaderProgramGLSL untexturedShaderProgram;
   /// Shader program for only the drawing depth of an opaque surface.
   ShaderProgramGLSL depthOpaqueShaderProgram;

   #if _DEBUG
      // Shader for debug display of a shadow map.
      ShaderProgramGLSL debugShadowMapShaderProgram;
   #endif

   friend class SceneInstanceForwardOpenGL;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__SCENEMANAGERFORWARDOPENGL_H__
