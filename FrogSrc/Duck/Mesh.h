#ifndef __FROG__DUCK__MESH_H__
#define __FROG__DUCK__MESH_H__

#include "FrogMemory.h"
#include "BSONIterator.h"
#include "Table.h"
#include "Duck/Drawable.h"

namespace Webfoot {
namespace Duck {

class MaterialInstance;
class Submesh;
class SubmeshInstance;
class Scene;
class SceneNode;
class SceneNodeMesh;
class SceneNodeMeshMorphTarget;
class VertexBuffer;
class IndexBuffer;
class VertexAttribute;
class MeshBone;
class SubmeshBone;
struct SubmeshBoneVertexInfluence;
class SubmeshInstanceBone;
class SubmeshMorphTarget;
struct SubmeshMorphTargetVertexOffsetPoint3F;
class SubmeshInstanceMorphTarget;

/// Submeshes of a mesh.
#define DUCK_MESH_SUBMESHES_KEY "Submeshes"
/// Center of the bounding sphere of a mesh.
#define DUCK_MESH_BOUNDING_SPHERE_CENTER_KEY "BoundingSphereCenter"
/// Radius of the bounding sphere of a mesh.
#define DUCK_MESH_BOUNDING_SPHERE_RADIUS_KEY "BoundingSphereRadius"
/// Name of the scene node for the root of the skeleton when using skinning.
#define DUCK_MESH_SKELETON_KEY "Skeleton"
/// Bone information that is specific to a mesh, but not to a submesh.
#define DUCK_MESH_BONES_KEY "Bones"

/// Initial material to use for a given submesh.
#define DUCK_SUBMESH_MATERIAL_KEY "Material"
/// Vertex attributes of a submesh.
#define DUCK_SUBMESH_VERTEX_ATTRIBUTES_KEY "VertexAttributes"
/// Vertex buffers of a submesh.
#define DUCK_SUBMESH_VERTEX_BUFFERS_KEY "VertexBuffers"
/// Index buffers of a submesh.
#define DUCK_SUBMESH_INDEX_BUFFERS_KEY "IndexBuffers"
/// Bone information that is specific to a submesh.
#define DUCK_SUBMESH_BONES_KEY "Bones"
/// Morph target information that is specific to a submesh.
#define DUCK_SUBMESH_MORPH_TARGETS_KEY "MorphTargets"

/// Name of the attribute.  (For example, "UVMap")
#define DUCK_VERTEX_ATTRIBUTE_NAME_KEY "Name"
/// Index of the object with the vertex data for the attribute.
#define DUCK_VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY "VertexBufferIndex"
/// Type of an individual component of data for a vertex.
#define DUCK_VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY "ComponentType"
/// How many components are in the data for a single vertex.  (For example,
/// 3 for a Point3F).
#define DUCK_VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY "ComponentCount"
/// Number of bytes from the beginning of the vertex buffer to the first
/// byte for the attribute.
#define DUCK_VERTEX_ATTRIBUTE_OFFSET_KEY "Offset"
/// Number of bytes from the beginning of the data for one vertex to the
/// beginning of the next.
#define DUCK_VERTEX_ATTRIBUTE_STRIDE_KEY "Stride"
/// True if the components should be normalized to [0, 1] or [-1, 1].  For
/// example, this would be true for vertex colors if the components were
/// [0, 255] but the meaning was [0, 1].
#define DUCK_VERTEX_ATTRIBUTE_NORMALIZED_KEY "Normalized"
/// Expected usage of the attribute.
#define DUCK_VERTEX_ATTRIBUTE_SEMANTIC_KEY "Semantic"
/// Index number to distinguish multiple attributes with the same semantic.
/// (For example, maybe 0 for the base color texture coordinates and 1 for
/// the ambient occlusion texture coordinates.)
#define DUCK_VERTEX_ATTRIBUTE_SEMANTIC_INDEX_KEY "SemanticIndex"

/// Raw vertex data.
#define DUCK_VERTEX_BUFFER_DATA_KEY "Data"

/// Data type for the individual indices.
#define DUCK_INDEX_BUFFER_COMPONENT_TYPE_KEY "ComponentType"
/// Number of indices.
#define DUCK_INDEX_BUFFER_INDEX_COUNT_KEY "IndexCount"
/// Type of primitive to draw.
#define DUCK_INDEX_BUFFER_PRIMITIVE_TYPE_KEY "PrimitiveType"
/// Raw index data.
#define DUCK_INDEX_BUFFER_DATA_KEY "Data"

/// Name of the bone associated with a MeshBone object.
#define DUCK_MESH_BONE_NAME_KEY "Name"
/// Inverse bind matrix for the rest pose of this object's bone.
#define DUCK_MESH_BONE_INVERSE_BIND_MATRIX_KEY "InverseBindMatrix"

/// Name of the bone associated with a SubmeshBone object.
#define DUCK_SUBMESH_BONE_NAME_KEY "Name"
/// Array of objects which specify how this bone influences individual
/// vertices.
#define DUCK_SUBMESH_BONE_VERTEX_INFLUENCES_KEY "VertexInfluences"

/// Name of the morph target associated with a SubmeshMorphTarget.
#define DUCK_SUBMESH_MORPH_TARGET_NAME_KEY "Name"
/// Data on how this morph target affects the positions of individual vertices.
#define DUCK_SUBMESH_MORPH_TARGET_POSITION_KEY "Position"
/// Data on how this morph target affects the normals of individual vertices.
#define DUCK_SUBMESH_MORPH_TARGET_NORMAL_KEY "Normal"
/// Data on how this morph target affects the smooth normals of individual vertices.
#define DUCK_SUBMESH_MORPH_TARGET_NORMAL_SMOOTH_KEY "NormalSmooth"
/// Data on how this morph target affects the tangents of individual vertices.
#define DUCK_SUBMESH_MORPH_TARGET_TANGENT_KEY "Tangent"

/// Name of the vertex attribute for smooth normals.
#define DUCK_VERTEX_ATTRIBUTE_NORMAL_SMOOTH_NAME "Smooth"
/// Name of the vertex attribute for wind information.
#define DUCK_VERTEX_ATTRIBUTE_COLOR_WIND_NAME "Wind"

//==============================================================================

/// Data types for the individual components of a vertex attribute.
enum VertexAttributeComponentType
{
   /// 32-bit floating point
   VERTEX_ATTRIBUTE_COMPONENT_TYPE_FLOAT32,
   /// Unsigned 8-bit integer.
   VERTEX_ATTRIBUTE_COMPONENT_TYPE_UINT8,
   VERTEX_ATTRIBUTE_COMPONENT_TYPE_COUNT
};

//==============================================================================

/// Expected usage types for a vertex attribute.
enum VertexAttributeSemantic
{
   /// Position of the vertex.
   VERTEX_ATTRIBUTE_SEMANTIC_POSITION,
   /// Normal vector of the vertex.
   VERTEX_ATTRIBUTE_SEMANTIC_NORMAL,
   /// Tangent vector of the vertex.
   VERTEX_ATTRIBUTE_SEMANTIC_TANGENT,
   /// Texture coordinate
   VERTEX_ATTRIBUTE_SEMANTIC_TEX_COORD,
   /// Vertex color
   VERTEX_ATTRIBUTE_SEMANTIC_COLOR,
   VERTEX_ATTRIBUTE_SEMANTIC_COUNT
};

//==============================================================================

/// Type of primitive to draw.
enum PrimitiveType
{
   /// Triangles
   PRIMITIVE_TYPE_TRIANGLE,
   PRIMITIVE_TYPE_COUNT
};

//==============================================================================

/// Data types for the individual indices in an index buffer.
enum IndexBufferComponentType
{
   /// Unsigned 16-bit integers.
   INDEX_BUFFER_COMPONENT_TYPE_UINT16,
   INDEX_BUFFER_COMPONENT_TYPE_COUNT
};

//==============================================================================

/// Data for a mesh shared across all instances of the mesh for a given
/// SceneManager.
class Mesh
{
public:
   Mesh();
   virtual ~Mesh() {}

   virtual void Init(Scene* _scene, BSONIterator meshSpecs);
   virtual void Deinit();

   /// Return the scene from which this mesh was loaded, if any.
   Scene* SceneGet() { return scene; }

   /// Return the number of submeshes in this mesh.
   int SubmeshCountGet() { return submeshes.SizeGet(); }
   /// Return the given submesh.
   Submesh* SubmeshGet(int submeshIndex);

   /// Return the bounding sphere of this object.
   Sphere BoundingSphereGet() { return boundingSphere; }

   /// Return the name of the scene node for the root of the skeleton when
   /// using skinning.
   const char* SkeletonNameGet() { return skeletonName; }

   /// Return the object for data for the given bone which is specific to the
   /// mesh, but not to the submesh.
   MeshBone* MeshBoneGet(const char* boneName);

protected:
   /// Return a new submesh.
   virtual Submesh* SubmeshCreate() = 0;

   /// Scene of which this mesh is a part.
   Scene* scene;
   /// Bounding sphere of this object.
   Sphere boundingSphere;
   /// Name of the scene node for the root of the skeleton when using skinning.
   const char* skeletonName;
   /// Collection of submeshes of which this mesh consists.
   Table<Submesh*> submeshes;
   /// Collection of bone information specific to a mesh, but not to a submesh.
   Table<MeshBone*> meshBones;
};

//==============================================================================

/// Data for a submesh is shared across all instances of the submesh for a given
/// SceneManager.
class Submesh
{
public:
   Submesh();
   virtual ~Submesh() {}

   virtual void Init(Mesh* _mesh, BSONIterator submeshSpecs);
   virtual void Deinit();

   /// Return the vertex attribute with the given semantic.  Use
   /// 'semanticIndex' to differentiate between multiple vertex attributes with
   /// the same semantic.
   VertexAttribute* VertexAttributeGetBySemantic(VertexAttributeSemantic semantic, int semanticIndex = 0);
   /// Return the vertex attribute with the given semantic and name.
   VertexAttribute* VertexAttributeGetBySemanticAndName(VertexAttributeSemantic semantic, const char* name);

   /// Return the given vertex buffer.
   VertexBuffer* VertexBufferGet(int vertexBufferIndex);

   /// Return the first index buffer of the given primitive type.
   IndexBuffer* IndexBufferGetByPrimitiveType(PrimitiveType primitiveType);

   /// Create a new vertex buffer.
   virtual VertexBuffer* VertexBufferCreate() = 0;
   /// Create a new index buffer.
   virtual IndexBuffer* IndexBufferCreate() = 0;
   /// Create a new submesh instance.
   virtual SubmeshInstance* SubmeshInstanceCreate() = 0;

   /// Return the mesh of which this is a part.
   Mesh* MeshGet() { return mesh; }
   /// Return the name of the material which should initially be used with this
   /// submesh.
   const char* MaterialNameGet() { return materialName; }

   /// Return the number of bones for which there is submesh-specific data.
   int SubmeshBoneCountGet() { return submeshBones.SizeGet(); }
   /// Return the object for submesh-specific data on the given bone.
   SubmeshBone* SubmeshBoneGet(int submeshBoneIndex);
   /// Return the object for submesh-specific data on the given bone.
   SubmeshBone* SubmeshBoneGet(const char* boneName);

   /// Return the number of morph targets for which there is submesh-specific
   /// data.
   int SubmeshMorphTargetCountGet() { return submeshMorphTargets.SizeGet(); }
   /// Return the objct for submesh-specific data on the given morph target.
   SubmeshMorphTarget* SubmeshMorphTargetGet(int submeshMorphTargetIndex);

   /// Return the number of vertices in the submesh.
   int VertexCountGet() { return vertexCount; }

protected:
   /// Mesh of which this is a part.
   Mesh* mesh;
   /// Name of the material which should initially be used with this submesh.
   const char* materialName;
   /// Number of vertices in the submesh.
   int vertexCount;
   /// Available vertex attributes for this submesh.
   Table<VertexAttribute*> vertexAttributes;
   /// Vertex buffers for this submesh.
   Table<VertexBuffer*> vertexBuffers;
   /// Index buffers for this submesh.
   Table<IndexBuffer*> indexBuffers;
   /// Bone information that is specific to a submesh.
   Table<SubmeshBone*> submeshBones;
   /// Morph target information that is specific to a submesh.
   Table<SubmeshMorphTarget*> submeshMorphTargets;
};

//==============================================================================

/// A single attribute of a vertex for a given submesh, like position or normal.
class VertexAttribute
{
public:
   VertexAttribute();

   virtual void Init(BSONIterator vertexAttributeSpecs);
   virtual void Deinit();

   /// Name of this attribute.  (For example, "UVMap")
   const char* name;
   /// Index of the object with the vertex data for this attribute.
   int vertexBufferIndex;
   /// Type of an individual component of data for a vertex.
   VertexAttributeComponentType componentType;
   /// How many components are in the data for a single vertex.  (For example,
   /// 3 for a Point3F).
   int componentCount;
   /// Number of bytes from the beginning of the vertex buffer to the first
   /// byte for this attribute.
   int offset;
   /// Number of bytes from the beginning of the data for one vertex to the
   /// beginning of the next.
   int stride;
   /// True if the components should be normalized to [0, 1] or [-1, 1].  For
   /// example, this would be true for vertex colors if the components were
   /// [0, 255] but the meaning was [0, 1].
   bool normalized;
   /// Expected usage of this attribute.
   VertexAttributeSemantic semantic;
   /// Index number to distinguish multiple attributes with the same semantic.
   /// (For example, maybe 0 for the base color texture coordinates and 1 for
   /// the ambient occlusion texture coordinates.)
   int semanticIndex;
};

//==============================================================================

/// Buffer of vertex data.
class VertexBuffer
{
public:
   VertexBuffer();
   virtual ~VertexBuffer() {}

   virtual void Init(BSONIterator vertexBufferSpecs);
   virtual void InitDynamic(size_t _bufferDataSize);
   virtual void Deinit();

   /// Flush the vertex data to the native object.
   virtual void Refresh() = 0;
   
   /// Raw vertex data.
   void* bufferData;
   /// Size of 'bufferData'.
   size_t bufferDataSize;
};

//==============================================================================

/// Buffer of index data.
class IndexBuffer
{
public:
   IndexBuffer();

   virtual void Init(BSONIterator indexBufferSpecs);
   virtual void Deinit();

   /// Type of primitive to draw.
   PrimitiveType primitiveType;
   /// Data type for the individual indices.
   IndexBufferComponentType componentType;
   /// Number of indices.
   int indexCount;
   /// Raw index data.
   void* bufferData;
   /// Size of 'bufferData'.
   size_t bufferDataSize;
};

//==============================================================================

/// Object for a specific instance of a submesh.
class SubmeshInstance : public Drawable
{
public:
   SubmeshInstance();

   virtual void Init(Submesh* _submesh, SceneNodeMesh* _sceneNodeMesh);
   virtual void Deinit();

   /// Return the submesh of which this is an instance.
   Submesh* SubmeshGet() { return submesh; }

   virtual SceneNode* DrawableSceneNodeGet() { return (SceneNode*)sceneNodeMesh; }
   virtual Point3F DrawableDepthSortPositionGet();
   virtual float DrawableDepthSortPriorityGet();

   /// Return true if the material for this submesh instance is fully opaque.
   bool FullyOpaqueCheck();
   /// Return true if the material for this submesh casts shadows.
   bool ShadowCasterCheck();

   /// Add the given object to the collection of bone data that is specific to
   /// this submesh instance.
   void SubmeshInstanceBoneAdd(SubmeshInstanceBone* submeshInstanceBone);

   /// Set whether the dynamic aspects of the submesh were updated for the
   /// current frame.
   void FrameRefreshedSet(bool _frameRefreshed) { frameRefreshed = _frameRefreshed; }

   /// Based on the latest available morph targets in the SceneNodeMesh, create
   /// a set of SubmeshInstanceMorphTarget objects.
   void SubmeshInstanceMorphTargetsRefresh();
   /// Return the number of SubmeshInstanceMorphTarget objects.
   int SubmeshInstanceMorphTargetCountGet() { return submeshInstanceMorphTargets.SizeGet(); }

   /// Return the MaterialInstance for drawing this SubmeshInstance.
   MaterialInstance* MaterialInstanceGet() { return materialInstance; }

protected:
   /// If this is a dynamic submesh, refresh it.
   void Refresh();

   /// Submesh of which this is an instance.
   Submesh* submesh;
   /// Scene node to which this instance belongs.
   SceneNodeMesh* sceneNodeMesh;
   /// MaterialInstance for drawing this SubmeshInstance.
   MaterialInstance* materialInstance;
   /// Buffer for position data to use for dynamic meshes, like for skinning
   /// and morph targets.
   VertexBuffer* dynamicPositionVertexBuffer;
   /// Buffer for normal data to use for dynamic meshes, like for skinning
   /// and morph targets.
   VertexBuffer* dynamicNormalVertexBuffer;
   /// Buffer for smooth normal data to use for dynamic meshes, like for skinning
   /// and morph targets.
   VertexBuffer* dynamicNormalSmoothVertexBuffer;
   /// Buffer for tangent data to use for dynamic meshes, like for skinning
   /// and morph targets.
   VertexBuffer* dynamicTangentVertexBuffer;
   /// Buffer used for vertex position data after morph targets are applied
   /// but before skinning.  This may not be used unless both morph targets
   /// and skinning are required.
   void* tempVertexPositions;
   /// Buffer used for vertex normal data after morph targets are applied
   /// but before skinning.  This may not be used unless both morph targets
   /// and skinning are required.
   void* tempVertexNormals;
   /// Buffer used for smooth vertex normal data after morph targets are applied
   /// but before skinning.  This may not be used unless both morph targets
   /// and skinning are required.
   void* tempVertexNormalsSmooth;
   /// Buffer used for vertex tangent data after morph targets are applied
   /// but before skinning.  This may not be used unless both morph targets
   /// and skinning are required.
   void* tempVertexTangents;
   /// Set whether the dynamic aspects of the submesh were updated for the current frame.
   bool frameRefreshed;
   /// Bone data specific to this instance of the submesh.
   Table<SubmeshInstanceBone*> submeshInstanceBones;
   /// Morph target data specific to this instance of the submesh.
   Table<SubmeshInstanceMorphTarget*> submeshInstanceMorphTargets;
};

//==============================================================================

/// Bone information that is specific to a mesh, but not to a submesh.
class MeshBone
{
public:
   MeshBone();

   void Init(BSONIterator specs);
   void Deinit();

   /// Name of the bone associated with this object.
   const char* name;
   /// Inverse bind matrix for the rest pose of this object's bone.
   Matrix43 inverseBindMatrix;
};

//==============================================================================

/// Bone information that is specific to a submesh.
class SubmeshBone
{
public:
   SubmeshBone();

   void Init(BSONIterator specs);
   void Deinit();

   /// Name of the bone associated with this object.
   const char* name;
   /// Number of items in the 'vertexInfluences' array.
   int vertexInfluenceCount;
   /// Array of objects which specify how this bone influences individual
   /// vertices.
   SubmeshBoneVertexInfluence* vertexInfluences;
};

//==============================================================================

/// Specifies the degree to which a given bone influences a given vertex.
struct SubmeshBoneVertexInfluence
{
   /// Index of the vertex affected by the bone.
   uint32 vertexIndex;
   /// Degree of influence of the bone's transform upon the given vertex.
   float32 weight;
};

//==============================================================================

/// Bone information for a specific instance of a submesh.
class SubmeshInstanceBone
{
public:
   SubmeshInstanceBone();

   void Deinit();

   /// SceneNode for the bone.
   SceneNode* boneNode;
   /// Data about the bone that is specific to the associated mesh, but not to
   /// the submesh.
   MeshBone* meshBone;
   /// Data about the bone that is specific to this submesh.
   SubmeshBone* submeshBone;
};

//==============================================================================

/// Morph target information that is specific to a submesh.
class SubmeshMorphTarget
{
public:
   SubmeshMorphTarget();

   void Init(BSONIterator specs);
   void Deinit();

   /// Name of the morph target associated with this object.
   const char* name;
   /// Number of items in the 'positionOffsets' array.
   int positionOffsetCount;
   /// Array of objects which specify how this morph target affects the
   /// positions of individual vertices.
   SubmeshMorphTargetVertexOffsetPoint3F* positionOffsets;
   /// Number of items in the 'normalOffsets' array.
   int normalOffsetCount;
   /// Array of objects which specify how this morph target affects the
   /// normals of individual vertices.
   SubmeshMorphTargetVertexOffsetPoint3F* normalOffsets;
   /// Number of items in the 'normalSmoothOffsets' array.
   int normalSmoothOffsetCount;
   /// Array of objects which specify how this morph target affects the
   /// smooth normals of individual vertices.
   SubmeshMorphTargetVertexOffsetPoint3F* normalSmoothOffsets;
   /// Number of items in the 'tangentOffsets' array.
   int tangentOffsetCount;
   /// Array of objects which specify how this morph target affects the
   /// tangents of individual vertices.
   SubmeshMorphTargetVertexOffsetPoint3F* tangentOffsets;

protected:
   /// Helper function for setting up the influence of the morph target for a
   /// given vertex attribute.
   void InitAttributePoint3FHelper(BSONIterator morphTargetSpecs, const char* offsetsKey,
      int* offsetCount, SubmeshMorphTargetVertexOffsetPoint3F** offsets);
};

//==============================================================================

/// Object for the way a morph target influences a given vertex attribute with
/// a type of Point3F.
struct SubmeshMorphTargetVertexOffsetPoint3F
{
   /// Index of the vertex affected by the morph target.
   uint32 vertexIndex;
   /// Change in vertex when the corresponding morph target is at full
   /// influence.
   Point3F offset;
};

//==============================================================================

/// Information specific to a combination of a submesh instance and a morph
/// target.  While this information could be found by searching at runtime,
/// caching it can be helpful when there are a lot of morph targets.
class SubmeshInstanceMorphTarget
{
public:
   SubmeshInstanceMorphTarget();

   void Init(SubmeshMorphTarget* _submeshMorphTarget, SceneNodeMeshMorphTarget* _sceneNodeMeshMorphTarget);
   void Deinit();

   /// Instance-independent submesh-specific data for this morph target.
   SubmeshMorphTarget* submeshMorphTarget;
   /// Instance-specific submesh-independent data for this morph target.
   SceneNodeMeshMorphTarget* sceneNodeMeshMorphTarget;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__MESH_H__
