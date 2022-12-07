#ifndef __FROG__DUCK__MATERIAL_H__
#define __FROG__DUCK__MATERIAL_H__

#include "FrogMemory.h"
#include "Screen.h"
#include "Texture.h"
#include "Table.h"

namespace Webfoot {

class JSONValue;

namespace Duck {

class MaterialInstance;
class MaterialTexture;
class MaterialInstanceTexture;
struct MaterialMeshConfiguration;
class Scene;
class SceneInstance;
class SceneNode;
class SceneNodeProjectorTexture;

/// Type of a given material.
#define DUCK_MATERIAL_TYPE_KEY "Type"

/// Filename of the texture to use for a material instance.
#define DUCK_MATERIAL_INSTANCE_TEXTURE_FILENAME_KEY "Filename"
/// Name of the UV Map to use with a given texture.
#define DUCK_MATERIAL_INSTANCE_TEXTURE_UV_MAP_KEY "UVMap"
/// Filtering/interpolation mode to use for this texture.
#define DUCK_MATERIAL_INSTANCE_TEXTURE_FILTER_MODE_KEY "FilterMode"
/// How to extend a texture beyond the texcoord range [0, 1] for the U axis.
#define DUCK_MATERIAL_INSTANCE_TEXTURE_WRAP_MODE_U_KEY "WrapModeU"
/// How to extend a texture beyond the texcoord range [0, 1] for the V axis.
#define DUCK_MATERIAL_INSTANCE_TEXTURE_WRAP_MODE_V_KEY "WrapModeV"

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
   TEXTURE_FILTER_MODE_COUNT
};

//=============================================================================

/// Ways of extending textures when the UV coordinates are outside the range of
/// 0 to 1.
enum TextureWrapMode
{
   TEXTURE_WRAP_MODE_INVALID = -1,
   TEXTURE_WRAP_MODE_REPEAT,
   TEXTURE_WRAP_MODE_CLAMP,
   TEXTURE_WRAP_MODE_COUNT
};

/// Return the texture wrap mode for the given name.  Return
/// TEXTURE_WRAP_MODE_INVALID if unsuccessful.
int TextureWrapModeStringToCode(const char* textureWrapModeName);

//==============================================================================

/// Base class for data for a material shared across all instances of the
/// material for a given SceneManager.
class Material
{
public:
   Material();
   virtual ~Material() {}

   virtual void Init(const char* _name, Scene* _scene, JSONValue* _specifications);
   virtual void Deinit();

   /// Return the name of this material.
   const char* NameGet() { return name; }
   /// Return the scene from which this material was loaded, if any.
   Scene* SceneGet() { return scene; }
   /// Return the specifications used to create this material and its
   /// instances.
   JSONValue* SpecificationsGet() { return specifications; }

   /// Return a new MaterialInstance for this material.
   virtual MaterialInstance* MaterialInstanceCreate() = 0;

protected:
   /// Name of this material.
   const char* name;
   /// Scene of which this mesh is a part.
   Scene* scene;
   /// Specifications used to create this material and its instances.
   JSONValue* specifications;
   /// Collection of textures used by this object.
   Table<MaterialTexture*> textures;
};

//==============================================================================

/// Instance of a given Material.
class MaterialInstance
{
public:
   MaterialInstance();
   virtual ~MaterialInstance() {}

   virtual void Init(Material* _material);
   virtual void Deinit();

   /// Return the number of passes needed to draw this material.
   virtual int PassCountGet(MaterialMeshConfiguration* materialMeshConfiguration) { (void)materialMeshConfiguration; return 1; }
   /// Prepare for a pass of drawing.
   virtual bool PrePass(int passIndex, SceneInstance* sceneInstance, MaterialMeshConfiguration* materialMeshConfiguration)
   {
      (void)passIndex; (void)sceneInstance; (void)materialMeshConfiguration;
      return true;
   }
   /// Clean up after a pass of drawing.
   virtual void PostPass(int passIndex) { (void)passIndex; }

   /// Return false if there's a chance this material will produce a fragment
   /// at less than full opacity on this frame.
   virtual bool FullyOpaqueCheck() = 0;
   /// Return true if this material casts shadows.
   virtual bool ShadowCasterCheck() = 0;
   /// Return true if this material is currently two-sided.
   virtual bool TwoSidedCheck() = 0;

   /// Return the Material of which this is an instance.
   Material* MaterialGet() { return material; }

protected:
   /// Material of which this is an instance.
   Material* material;
   /// Collection of textures used by this object.
   Table<MaterialInstanceTexture*> textures;
};

//==============================================================================

/// Data specific to a given usage of a texture in a given material.
class MaterialTexture
{
public:
   MaterialTexture();

   virtual void Init(Material* _material, JSONValue* specifications, bool premultiplyAlpha);
   virtual void Deinit();

   /// Return the texture to be sampled.
   Texture* TextureGet() { return texture; }

   /// Return the name of the UV Map channel to use when sampling this texture.
   const char* UVMapNameGet() { return uvMapName; }

   /// Set which TexCoord channel index should be used when sampling this
   /// texture.
   void TexCoordChannelIndexSet(int _texCoordChannelIndex) { texCoordChannelIndex = _texCoordChannelIndex; }
   /// Return which TexCoord channel index should be used when sampling this
   /// texture.
   int TexCoordChannelIndexGet() { return texCoordChannelIndex; }

protected:
   /// Texture to be sampled.
   Texture* texture;
   /// Name of the UV Map channel to use when sampling this texture.
   const char* uvMapName;
   /// Which TexCoord channel index should be used when sampling this texture.
   int texCoordChannelIndex;
   /// How textures should be filtered/interpolated.
   TextureFilterMode filterMode;
   /// How to extend a texture beyond the texcoord range [0, 1] for the U axis.
   TextureWrapMode wrapModeU;
   /// How to extend a texture beyond the texcoord range [0, 1] for the V axis.
   TextureWrapMode wrapModeV;
};

//==============================================================================

/// Data specific to a given usage of a texture in a given instance of a
/// material.
class MaterialInstanceTexture
{
public:
   MaterialInstanceTexture();

   virtual void Init(MaterialInstance* _materialInstance, MaterialTexture* _materialTexture);
   virtual void Deinit();

   /// Return the MaterialTexture for which this is an instance;
   MaterialTexture* MaterialTextureGet() { return materialTexture; }

   /// Set the offset to be applied to the texture coordiantes.
   void TexCoordOffsetSet(const Point2F& _texCoordOffset) { texCoordOffset = _texCoordOffset; }
   /// Return the offset to be applied to the texture coordiantes.
   Point2F TexCoordOffsetGet() { return texCoordOffset; }
   /// Set the scale to be applied to the texture coordiantes.
   void TexCoordScaleSet(const Point2F& _texCoordScale) { texCoordScale = _texCoordScale; }
   /// Return the scale to be applied to the texture coordiantes.
   Point2F TexCoordScaleGet() { return texCoordScale; }

protected:
   /// MaterialInstance to which this belongs.
   MaterialInstance* materialInstance;
   /// MaterialTexture for which this is an instance;
   MaterialTexture* materialTexture;
   /// Offset to be applied to the texture coordiantes.
   Point2F texCoordOffset;
   /// Scale to be applied to the texture coordiantes.
   Point2F texCoordScale;
};

//==============================================================================

/// Allows materials to instruct meshes on how to provide vertex data.
struct MaterialMeshConfiguration
{
   enum
   {
      /// Maximum number of UV Map attributes
      TEX_COORD_COUNT_MAX = Screen::VERTEX_ATTRIBUTE_COUNT_MAX - 1,
      /// Maximum number of vertex color attributes
      VERTEX_COLOR_COUNT_MAX = Screen::VERTEX_ATTRIBUTE_COUNT_MAX - 1
   };

   enum Mode
   {
      /// When the material should be displayed as usual.
      MODE_REGULAR,
      /// When only the depth should be included.  This is used for shadows.
      MODE_DEPTH,
      /// When a texture is to be projected onto the material.
      MODE_PROJECTOR_TEXTURE
   };

   /// Helper struct for pairing a channel name with an vertex attribute index.
   struct ChannelAttributeIndex
   {
      /// NULL to exclude
      const char* channelName;
      /// -1 to exclude
      int attributeIndex;
   };

   /// Return to the default configuration.
   void Reset()
   {
      mode = MODE_REGULAR;
      useGeometryInstancing = false;
      useGeometryInstancingFallback = false;
      sceneNodeProjectorTexture = NULL;
      projectorMatrix = NULL;
      projectorTransform = NULL;
      lightProjectionMatrix = NULL;
      lightViewMatrix = NULL;
      normalAttributeIndex = -1;
      normalSmoothAttributeIndex = -1;
      tangentAttributeIndex = -1;
      geometryInstanceTransformAttributeIndexFirst = -1;
      for(int texCoordIndex = 0; texCoordIndex < TEX_COORD_COUNT_MAX; texCoordIndex++)
      {
         ChannelAttributeIndex* channelAttributeIndex = &(texCoordAttributeIndices[texCoordIndex]);
         channelAttributeIndex->channelName = NULL;
         channelAttributeIndex->attributeIndex = -1;
      }
      for(int colorIndex = 0; colorIndex < VERTEX_COLOR_COUNT_MAX; colorIndex++)
      {
         ChannelAttributeIndex* channelAttributeIndex = &(colorAttributeIndices[colorIndex]);
         channelAttributeIndex->channelName = NULL;
         channelAttributeIndex->attributeIndex = -1;
      }
   }

   /// How the material should be applied.
   Mode mode;
   /// True if the user of the material is requesting geometry instancing when
   /// drawing.
   bool useGeometryInstancing;
   /// True in cases where hardware geometry instancing has been requested, but
   /// can't be performed.  The system should fall back on doing multiple draw
   /// calls.
   bool useGeometryInstancingFallback;
   /// Projection matrix to use for the light's perspective for making shadow
   /// maps.  (used with MODE_DEPTH)
   const Matrix44* lightProjectionMatrix;
   /// View matrix to use for the light's perspective for making shadow maps.
   /// (used with MODE_DEPTH)
   const Matrix43* lightViewMatrix;
   /// Node representing the projector of a texture.  (used with MODE_PROJECTOR_TEXTURE)
   SceneNodeProjectorTexture* sceneNodeProjectorTexture;
   /// Projects from world space to texture space.  (used with MODE_PROJECTOR_TEXTURE)
   const Matrix44* projectorMatrix;
   /// Cached world space transform of 'sceneNodeProjectorTexture'  (used with MODE_PROJECTOR_TEXTURE)
   const Matrix43* projectorTransform;
   /// Vertex attribute index to use for vertex normals.  (-1 to exclude)
   int normalAttributeIndex;
   /// Vertex attribute index to use for smooth vertex normals.  (-1 to exclude)
   int normalSmoothAttributeIndex;
   /// Vertex attribute index to use for vertex tangents.  (-1 to exclude)
   int tangentAttributeIndex;
   /// First of three consecutive vertex attribute indices for the rows of the
   /// instance-specific transform matrix.  (-1 to exclude)
   int geometryInstanceTransformAttributeIndexFirst;
   /// Which vertex attribute indices to use for the different UV maps.
   ChannelAttributeIndex texCoordAttributeIndices[TEX_COORD_COUNT_MAX];
   /// Which vertex attribute indices to use for the different vertex color
   /// channels.
   ChannelAttributeIndex colorAttributeIndices[VERTEX_COLOR_COUNT_MAX];
   /// See 'useGeometryInstancingFallback'.  This matrix provides the local
   /// transform of the current instance being processed.
   Matrix43 geometryInstancingFallbackTransformRelative;
};

//==============================================================================

} //namespace Duck {
} //namespace Webfoot {

#endif //#ifndef __FROG__DUCK__MATERIAL_H__
