#ifndef __FROG__DUCK__OPENGL__MATERIALSTANDARDFORWARDOPENGL_H__
#define __FROG__DUCK__OPENGL__MATERIALSTANDARDFORWARDOPENGL_H__

#include "FrogMemory.h"
#include "ShaderProgramGLSL.h"
#include "Duck/MaterialStandard.h"
#include "Duck/OpenGL/SceneForwardOpenGL.h"

namespace Webfoot {
namespace Duck {

class MaterialInstanceStandardForwardOpenGL;

//==============================================================================

/// General-purpose material.
class MaterialStandardForwardOpenGL : public MaterialStandard
{
public:
   static void SharedInit();
   static void SharedDeinit();

   MaterialStandardForwardOpenGL();

   virtual void Init(const char* _name, Scene* _scene, JSONValue* _specifications);
   virtual void Deinit();

   virtual MaterialInstance* MaterialInstanceCreate();
   virtual MaterialTexture* MaterialTextureCreate();
   virtual MaterialInstanceTexture* MaterialInstanceTextureCreate();

   static Material* Create() { return frog_new MaterialStandardForwardOpenGL(); }
   
   typedef MaterialStandard Inherited;

   friend class MaterialInstanceStandardForwardOpenGL;

protected:
   /// Helper function for initializing a shader for drawing depth.
   static ShaderProgramGLSL* ShaderProgramDepthInitHelper(const char* debugName,
      bool windApply, AlphaMode alphaMode, bool useGeometryInstancing);
   /// Helper to 'ShaderProgramDepthInitHelper'.
   static bool ShaderProgramDepthInitHelperHelper(ShaderProgramGLSL* _shaderProgram,
      const char* debugName, bool windApply, AlphaMode alphaMode, bool useGeometryInstancing);

   /// Helper function for initializing a shader for texture projection.
   static ShaderProgramGLSL* ShaderProgramProjectorTextureInitHelper(const char* debugName,
      bool windApply, AlphaMode alphaMode, bool useGeometryInstancing);
   /// Helper to 'ShaderProgramInitHelper'.
   static bool ShaderProgramProjectorTextureInitHelperHelper(ShaderProgramGLSL* _shaderProgram,
      const char* debugName, bool windApply, AlphaMode alphaMode, bool useGeometryInstancing);

   /// Add wind variables and functions to the vertex and fragment shaders.
   /// Return true if successful.
   static bool WindDeclarationsAdd(ShaderProgramGLSL* shaderProgram);

   /// Set up the shader program based on the loaded settings.
   void ShaderProgramsInit();
   /// Helper function for initializing a shader.
   ShaderProgramGLSL* ShaderProgramInitHelper(bool useGeometryInstancing);
   /// Helper function for initializing a shader for drawing an outline.
   ShaderProgramGLSL* ShaderProgramOutlineInitHelper(bool useGeometryInstancing);
   /// Helper to 'ShaderProgramInitHelper'.
   bool ShaderProgramInitHelperHelper(ShaderProgramGLSL* _shaderProgram, bool useGeometryInstancing);
   /// Helper to 'ShaderProgramOutlineInitHelper'.
   bool ShaderProgramOutlineInitHelperHelper(ShaderProgramGLSL* _shaderProgram, bool useGeometryInstancing);
   /// Clean up all shaders for this material.
   void ShaderProgramsDeinit();

   /// Shader program used to draw this material.
   ShaderProgramGLSL* shaderProgram;
   /// Shader program for instanced drawing.
   ShaderProgramGLSL* shaderProgramGeometryInstancing;
   /// Shader program used to draw an outline.
   ShaderProgramGLSL* shaderProgramOutline;
   /// Shader program used to draw an outline with instanced drawing.
   ShaderProgramGLSL* shaderProgramOutlineGeometryInstancing;

   /// Shared shader program for drawing depth for a fully opaque surface.
   static ShaderProgramGLSL* shaderProgramDepthOpaque;
   /// Shared shader for drawing depth for a fully opaque geometry instanced
   /// surface.
   static ShaderProgramGLSL* shaderProgramDepthOpaqueGeometryInstancing;
   /// Shared shader for drawing depth for a surface using alpha clipping.
   static ShaderProgramGLSL* shaderProgramDepthAlphaClip;
   /// Shared shader for drawing depth for a geometry instanced surface using
   /// alpha clipping.
   static ShaderProgramGLSL* shaderProgramDepthAlphaClipGeometryInstancing;
   /// Shared shader program for drawing depth for a fully opaque surface
   /// affected by wind.
   static ShaderProgramGLSL* shaderProgramDepthWindOpaque;
   /// Shared shader for drawing depth for a fully opaque geometry instanced
   /// surface affected by wind.
   static ShaderProgramGLSL* shaderProgramDepthWindOpaqueGeometryInstancing;
   /// Shared shader for drawing depth for a surface using alpha clipping
   /// and affected by wind.
   static ShaderProgramGLSL* shaderProgramDepthWindAlphaClip;
   /// Shared shader for drawing depth for a geometry instanced surface using
   /// alpha clipping and affected by wind.
   static ShaderProgramGLSL* shaderProgramDepthWindAlphaClipGeometryInstancing;

   /// Shared shader for projecting a texture onto a fully opaque surface.
   static ShaderProgramGLSL* shaderProgramProjectorTextureOpaque;
   /// Shared shader for projecting a texture onto a fully opaque geometry
   /// instanced surface.
   static ShaderProgramGLSL* shaderProgramProjectorTextureOpaqueGeometryInstancing;
   /// Shared shader for projecting a texture onto a surface using alpha
   /// clipping.
   static ShaderProgramGLSL* shaderProgramProjectorTextureAlphaClip;
   /// Shared shader for projecting a texture onto a geometry instanced surface
   /// using alpha clipping.
   static ShaderProgramGLSL* shaderProgramProjectorTextureAlphaClipGeometryInstancing;
   /// Shared shader for projecting a texture onto a surface using
   /// alpha-to-coverage.
   static ShaderProgramGLSL* shaderProgramProjectorTextureAlphaToCoverage;
   /// Shared shader for projecting a texture onto a geometry instanced surface
   /// using alpha-to-coverage.
   static ShaderProgramGLSL* shaderProgramProjectorTextureAlphaToCoverageGeometryInstancing;
   /// Shared shader for projecting a texture onto a fully opaque surface affected by wind.
   static ShaderProgramGLSL* shaderProgramProjectorTextureWindOpaque;
   /// Shared shader for projecting a texture onto a fully opaque geometry
   /// instanced surface and affected by wind.
   static ShaderProgramGLSL* shaderProgramProjectorTextureWindOpaqueGeometryInstancing;
   /// Shared shader for projecting a texture onto a surface using alpha
   /// clipping and affected by wind.
   static ShaderProgramGLSL* shaderProgramProjectorTextureWindAlphaClip;
   /// Shared shader for projecting a texture onto a geometry instanced surface
   /// using alpha clipping and affected by wind.
   static ShaderProgramGLSL* shaderProgramProjectorTextureWindAlphaClipGeometryInstancing;
   /// Shared shader for projecting a texture onto a surface using
   /// alpha-to-coverage and affected by wind.
   static ShaderProgramGLSL* shaderProgramProjectorTextureWindAlphaToCoverage;
   /// Shared shader for projecting a texture onto a geometry instanced surface
   /// using alpha-to-coverage and affected by wind.
   static ShaderProgramGLSL* shaderProgramProjectorTextureWindAlphaToCoverageGeometryInstancing;
};

//==============================================================================

/// Instance of the general-purpose material.
class MaterialInstanceStandardForwardOpenGL : public MaterialInstanceStandard
{
public:
   virtual void Init(Material* _material);

   virtual int PassCountGet(MaterialMeshConfiguration* materialMeshConfiguration);
   virtual bool PrePass(int passIndex, SceneInstance* sceneInstance,
      MaterialMeshConfiguration* materialMeshConfiguration);
   virtual void PostPass(int passIndex);

   virtual bool FullyOpaqueCheck();

   typedef MaterialInstanceStandard Inherited;

protected:
   /// Prepare for a pass of displaying the material normally.
   bool ModeRegularPrePass(int passIndex, SceneInstance* sceneInstance,
      MaterialMeshConfiguration* materialMeshConfiguration);
   /// Prepare for a pass of drawing an outline.
   bool ModeRegularOutlinePrePass(int passIndex, SceneInstance* sceneInstance,
      MaterialMeshConfiguration* materialMeshConfiguration);
   /// Prepare for a pass of drawing depth.
   bool ModeDepthPrePass(int passIndex, SceneInstance* sceneInstance,
      MaterialMeshConfiguration* materialMeshConfiguration);
   /// Prepare for a pass of projecting a texture.
   bool ModeProjectorTexturePrePass(int passIndex, SceneInstance* sceneInstance,
      MaterialMeshConfiguration* materialMeshConfiguration);
   /// Set the uniforms of the given shader for the wind.  Return true if successful.
   bool WindUniformsSet(ShaderProgramGLSL* shaderProgram, SceneInstance* sceneInstance,
      MaterialStandardForwardOpenGL* _material);

   /// Number of passes needed to draw with this material.
   int passCount;
   /// Index of a dedicated pass on which to do the other back faces of a
   /// two-sided material.
   int backSidePassIndex;
   /// Index of the pass on which an outline should be drawn.
   int outlinePassIndex;
   /// True if face culling should be re-enabled when cleaning up after this
   /// pass.
   bool restoreFaceCullingEnabled;
   /// True if face culling should be set to back faces when cleaning up after
   /// this pass.
   bool restoreBackfaceCulling;
   /// True if the depth test function should be set to its usual value when
   /// cleaning up after this pass.
   bool restoreDepthFunction;
   /// True if depth test writing should be restored when cleaning up after
   /// this pass.
   bool restoreDepthWriting;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__OPENGL__MATERIALSTANDARDFORWARDOPENGL_H__
