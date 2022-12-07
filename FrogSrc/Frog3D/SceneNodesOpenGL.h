#ifndef __FROG__FROG3D__SCENENODESOPENGL_H__
#define __FROG__FROG3D__SCENENODESOPENGL_H__

#include "FrogMemory.h"
#include "SceneNodesCommon.h"
#include "ShaderProgramGLSL.h"

namespace Webfoot {

//=============================================================================

/// Allows materials to instruct meshes on how to provide vertex data.
struct MaterialMeshConfiguration
{
   /// Return to the default configuration.
   void Reset()
   {
      normalAttributeIndex = -1;
      for(int mapChannelIndex = 0; mapChannelIndex < FROG3D_MAP_CHANNEL_COUNT_MAX; mapChannelIndex++)
         mapChannelAttributeIndices[mapChannelIndex] = -1;
   }

   /// Vertex attribute index to use for vertex normals.  (-1 to exclude)
   int normalAttributeIndex;
   /// Vertex attribute indices to use for different map channels (-1 to exclude)
   int mapChannelAttributeIndices[FROG3D_MAP_CHANNEL_COUNT_MAX];
};

//=============================================================================

/// OpenGL implementation of MeshShapeSceneNode.
class MeshShapeSceneNode : public MeshShapeSceneNodeCommon
{
public:
   virtual void Render(RenderData* renderData, SceneNode* renderNode,
      MaterialSceneNode* material, AnimationState* materialAnimationState);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) MeshShapeSceneNode(); }

   typedef MeshShapeSceneNodeCommon Inherited;
};

//=============================================================================

/// Allows addition of OpenGL-specific data for each submesh.
struct MeshShapeSubmesh : public MeshShapeSubmeshCommon
{
public:
   MeshShapeSubmesh(SceneLoadData* input) : Inherited(input) {}
   virtual ~MeshShapeSubmesh() {}

   typedef MeshShapeSubmeshCommon Inherited;
};

//=============================================================================

/// OpenGL implementation of PhysiqueShapeSceneNode.
class PhysiqueShapeSceneNode : public PhysiqueShapeSceneNodeCommon
{
public:
   virtual void Render(RenderData* renderData, SceneNode* renderNode,
      MaterialSceneNode* material, AnimationState* materialAnimationState);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) PhysiqueShapeSceneNode(); }

   typedef PhysiqueShapeSceneNodeCommon Inherited;
};

//=============================================================================

/// Allows addition of OpenGL-specific data for each submesh.
struct PhysiqueShapeSubmesh : public PhysiqueShapeSubmeshCommon
{
public:
   PhysiqueShapeSubmesh(SceneLoadData* input, PhysiqueShapeData* physiqueShapeData) :
      Inherited(input, physiqueShapeData) {}
   virtual ~PhysiqueShapeSubmesh() {}

   typedef PhysiqueShapeSubmeshCommon Inherited;
};

//=============================================================================

/// OpenGL implementation of SpriteShapeSceneNode.
class SpriteShapeSceneNode : public SpriteShapeSceneNodeCommon
{
public:
   virtual void Render(RenderData* renderData, SceneNode* renderNode,
      MaterialSceneNode* material, AnimationState* materialAnimationState);

   typedef SpriteShapeSceneNodeCommon Inherited;
};

//=============================================================================

/// OpenGL implementation of TextShapeSceneNode.
class TextShapeSceneNode : public TextShapeSceneNodeCommon
{
public:
   virtual void Render(RenderData* renderData, SceneNode* renderNode,
      MaterialSceneNode* material, AnimationState* materialAnimationState);

   typedef TextShapeSceneNodeCommon Inherited;
};


//=============================================================================

/// OpenGL implementation of TextureSceneNode.
class TextureSceneNode : public TextureSceneNodeCommon
{
public:
   /// Prepare to use the associated texture on the given texture unit.
   void SetupTexture(int textureUnitIndex);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) TextureSceneNode(); }

   typedef TextureSceneNodeCommon Inherited;
};

//=============================================================================

/// OpenGL implementation of StandardMaterialSceneNode.
class StandardMaterialSceneNode : public StandardMaterialSceneNodeCommon
{
public:
   /// Set up anything shared by all instances of this material.
   static void SharedInit();
   /// Clean up anything shared by all instances of this material.
   static void SharedDeinit();
   
   virtual void Init(SceneLoadData* input);
   virtual void Deinit();

   virtual size_t PassCountGet(RenderData* /*renderData*/) const { return (size_t)passCount; }
   virtual void MaterialPrePass(RenderData* renderData, size_t passIndex,
      SceneNode* renderNode, MaterialMeshConfiguration* materialMeshConfiguration, AnimationState* materialAnimationState);
   virtual void MaterialPostPass(RenderData* /*renderData*/, size_t /*passIndex*/,
      SceneNode* /*renderNode*/, AnimationState* /*materialAnimationState*/);

   /// Return the primitive type to use for the given pass of this material.
   virtual PrimitiveType PrimitiveTypeForPassGet(RenderData* renderData, size_t passIndex,
      SceneNode* renderNode);

   static SceneNode* Create(HeapID heapID) { return frog_new_ex(heapID) StandardMaterialSceneNode(); }

   /// Return the ShaderPipeline object for drawing silhouette edges.
   static ShaderProgramGLSL* SilhouetteShaderProgramGet() { return silhouetteShaderProgram; }

   typedef StandardMaterialSceneNodeCommon Inherited;

protected:
   enum
   {
      /// Maximum number of lights to apply to a given submesh at once.
      LIGHT_COUNT_MAX = 8
   };

   /// Set up the shader program based on the loaded settings.
   void ShaderProgramInit();
   void ShaderProgramDeinit() { shaderProgram.Deinit(); }

   /// Shader program used to draw this material instance.
   ShaderProgramGLSL shaderProgram;
   /// Number of passes needed to draw with this material.
   int passCount;
   /// Index of the pass on which to do the other side of a two-sided material.
   int backSidePassIndex;
   /// Index of the pass on which to draw the silhouette edges.
   int silhouettePassIndex;

   /// Shader program used to draw silhouette edges.
   static ShaderProgramGLSL* silhouetteShaderProgram;
};

//=============================================================================

/// OpenGL implementation of TextureProjectorSceneNode.
class TextureProjectorSceneNode : public TextureProjectorSceneNodeCommon
{
public:
   /// Apply the projector to the given scene.
   virtual void ApplyToScene(SceneNode* rootSceneNode, RenderData* renderData);

   typedef TextureProjectorSceneNodeCommon Inherited;
};

//=============================================================================

/// OpenGL implementation of TextureProjectorMaterialSceneNode.
class TextureProjectorMaterialSceneNode : public TextureProjectorMaterialSceneNodeCommon
{
public:
   /// Set up anything shared by all instances of this material.
   static void SharedInit();
   /// Clean up anything shared by all instances of this material.
   static void SharedDeinit();

   virtual void MaterialPrePass(RenderData* renderData, size_t passIndex,
      SceneNode* renderNode, MaterialMeshConfiguration* materialMeshConfiguration, AnimationState* materialAnimationState);
   virtual void MaterialPostPass(RenderData* renderData, size_t passIndex,
      SceneNode* renderNode, AnimationState* materialAnimationState);

protected:
   typedef TextureProjectorMaterialSceneNodeCommon Inherited;

   /// Shader program used to draw this material instance.
   static ShaderProgramGLSL shaderProgram;

   friend class TextureProjectorSceneNode;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENENODESOPENGL_H__
