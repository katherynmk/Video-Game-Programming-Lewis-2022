#ifndef __FROG__FROG3D__SCENEMANAGERCOMMON_H__
#define __FROG__FROG3D__SCENEMANAGERCOMMON_H__

#include "FrogMemory.h"
#include "Table.h"
#include "FileManager.h"

namespace Webfoot {

class SceneNode;
class SceneLoadData;
class RenderSceneNode;

//=============================================================================

/// Base class for SceneManager implementations.
class SceneManagerCommon
{
public:
   SceneManagerCommon();

   void Init();
   void Deinit();

   static SceneNode* Load(const char* fileName, FileManager* _fileManager = theFiles);
   static SceneNode* Load(SceneLoadData* input);

   void QueueNodeForRelease(SceneNode* node);

   /// A function for replacing a string if needed.  'originalString' is the
   /// string that may need to be replaced.  'newString' is a buffer into
   /// which a replacement string will be written if needed.  'newStringBufferSize'
   /// is the size of that buffer, including the space for the null-terminator.
   /// If a replacement string is written to 'newString', the function will return
   /// true.  If no changes are needed, it will return false, and the
   /// 'originalString' should be used by the caller rather than 'newString'.
   typedef bool (*StringModifier)(const char* originalString, char* newString, size_t newStringBufferSize);
   
   /// Set the StringModifier function to be used for loading alternate textures.
   /// The function should return true if a different filename should be used.
   /// See StringModifier for details.
   /// Use 'NULL' if you want to stop using a StringModifier.
   /// By default, the system will always use the requested filename as-is.
   void SetTextureFilenameModifier(StringModifier _textureFilenameModifier) 
   {
      textureFilenameModifier = _textureFilenameModifier;
   }
   /// See SetTextureFilenameModifier
   StringModifier GetTextureFilenameModifier() { return textureFilenameModifier; }

   /// Set to true if the whole framebuffer should be used, regardless of the
   /// Screen's OutputScaling.  Set to false to leave the scissor test
   /// enabled while drawing.
   void SetUseEntireFramebuffer(bool _useEntireFramebuffer) { useEntireFramebuffer = _useEntireFramebuffer; }
   /// See 'SetUseEntireFramebuffer'
   bool GetUseEntireFramebuffer() { return useEntireFramebuffer; }

   /// Set a hint for whether materials should default to using per-pixel
   /// lighting or per-vertex lighting when they are initialized.  This setting
   /// is not necessarily followed on all platforms and situations.
   void SetPixelLightingDefault(bool _pixelLightingDefault) { pixelLightingDefault = _pixelLightingDefault; }
   /// Return the current hint setting for whether to default to per-vertex
   /// lighting or per-pixel lighting.
   bool GetPixelLightingDefault() { return pixelLightingDefault; }

   /// Set a hint for the thickness of silhouette edges used by materials as
   /// they are initialized.  This setting is not necessarily followed on all
   /// platforms and situations.
   void SetSilhouetteEdgeWidthDefault(float _silhouetteEdgeWidthDefault) { silhouetteEdgeWidthDefault = _silhouetteEdgeWidthDefault; }
   /// See 'SetSilhouetteEdgeWidth'.
   float GetSilhouetteEdgeWidthDefault() { return silhouetteEdgeWidthDefault; }

   /// Clean up the contents of the nodeCleanupList.  This is for use by Scene only.
   void NodeCleanupListProcess();

protected:
   /// Optional function that allows the game to load alternate textures.
   StringModifier textureFilenameModifier;

   /// True if the whole framebuffer should be used, regardless of the Screen's
   /// OutputScaling.
   bool useEntireFramebuffer;
   /// Hint for whether materials should default to using per-pixel
   /// lighting or per-vertex lighting when they are initialized.  This setting
   /// is not necessarily followed on all platforms and situations.
   bool pixelLightingDefault;
   /// Hint for the thickness of silhouette edges used by materials as
   /// they are initialized.  This setting is not necessarily followed on all
   /// platforms and situations.
   float silhouetteEdgeWidthDefault;
   /// True if the ParticleManager3D should be deinitialized when this is
   /// deinitialized.
   bool deinitParticleManager3D;
   
   Table<SceneNode*> nodeCleanupList;
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENEMANAGERCOMMON_H__
