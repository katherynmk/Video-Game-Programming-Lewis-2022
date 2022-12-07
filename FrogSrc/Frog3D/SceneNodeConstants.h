#ifndef __FROG__FROG3D__SCENENODECONSTANTS_H__
#define __FROG__FROG3D__SCENENODECONSTANTS_H__

// Ordinarily, we'd include Memory.h here, but this header is also included by
// the 3dsMaxExporter, and this doesn't have any dependancies anyway.
//#include "FrogMemory.h"

namespace Webfoot {

/// Current version number of the format for Frog scene files.
#define FROG_SCENE_FILE_FORMAT_VERSION 0

//=============================================================================

enum SceneNodeType
{
   SceneNodeType_Unknown                     = 0,


   SceneNodeType_FixedGroupSceneNode         = 0x10,
   SceneNodeType_TransformFixedGroupSceneNode,

   SceneNodeType_Render,
   SceneNodeType_Spline,
   SceneNodeType_Helper,
   SceneNodeType_Bone,
   SceneNodeType_Skeleton,
   SceneNodeType_MorphAnimation,

   SceneNodeType_ConstantTransformController = 0x20,
   SceneNodeType_PRSTransformController,
   SceneNodeType_SequenceTransformController,
   SceneNodeType_BillboardTransformController,
   SceneNodeType_UprightBillboardTransformController,
   SceneNodeType_BoneTransformController,

   SceneNodeType_ConstantPositionController  = 0x30,
   SceneNodeType_XYZPositionController,
   SceneNodeType_PathPositionController,

   SceneNodeType_ConstantRotationController  = 0x40,
   SceneNodeType_EulerXYZRotationController,


   SceneNodeType_ConstantScaleController     = 0x50,


   SceneNodeType_ConstantFloatController     = 0x60,
   SceneNodeType_BezierFloatController,
   SceneNodeType_LinearFloatController,


   SceneNodeType_MeshShape                   = 0x70,
   SceneNodeType_PhysiqueShape,
   SceneNodeType_AnimatedShape,


   SceneNodeType_BezierPath                  = 0x80,


   SceneNodeType_MultiSubMaterial            = 0x90,
   SceneNodeType_StandardMaterial,
   SceneNodeType_AnimatedMaterial,
   //SceneNodeType_CompositeMaterial,


   SceneNodeType_Camera                      = 0xA0,
   SceneNodeType_Light,
   SceneNodeType_Texture
};

//=============================================================================

/// Ways to encode the elements of a vertex attribute.
enum VertexAttributeDataType
{
   VERTEX_ATTRIBUTE_DATA_TYPE_NORM_UINT8,
   VERTEX_ATTRIBUTE_DATA_TYPE_FLOAT32
};

//=============================================================================

/// Ways to encode the elements of an index buffer.
enum IndexDataType
{
   INDEX_DATA_TYPE_UINT16
};

//=============================================================================

enum LightType
{
   LIGHT_TYPE_DIRECTIONAL
};

//=============================================================================

/// Forms of texture filtering.
enum TextureFilterMode
{
   /// Point filtering
   TEXTURE_FILTER_MODE_POINT,
   /// Biliniear filtering
   TEXTURE_FILTER_MODE_BILINEAR,
   /// Trilinear filtering
   TEXTURE_FILTER_MODE_TRILINEAR,
   /// Anisotropic filtering.  This is not necessarily supported on all
   /// platforms.
   TEXTURE_FILTER_MODE_ANISOTROPIC
};

//=============================================================================

/// Ways of wrapping textures when the UV coordinates are outside the range of
/// 0 to 1.
enum TextureWrapMode
{
   TEXTURE_WRAP_MODE_REPEAT,
   TEXTURE_WRAP_MODE_CLAMP,
   TEXTURE_WRAP_MODE_MIRROR
};

//=============================================================================

} // namespace Webfoot {

#endif //#ifndef __FROG__FROG3D__SCENENODECONSTANTS_H__
