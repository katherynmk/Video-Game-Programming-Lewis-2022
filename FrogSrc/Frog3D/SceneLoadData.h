#ifndef __FROG__FROG3D__SCENELOADDATA_H__
#define __FROG__FROG3D__SCENELOADDATA_H__

#include "FrogMemory.h"
#include "File.h"
#include "FileManager.h"

namespace Webfoot {

class SceneNode;

//=============================================================================

/// Class for loading 3D scenes fom files.
class SceneLoadData
{
public:
   SceneLoadData(const char* fileName, FileManager* _fileManager = theFiles);
   ~SceneLoadData();

   bool WasOpenSuccessful() { return file != NULL; }

   void ReadBlock(void* data, size_t length) { file->Read(data, length); }

   template<typename T>
   void Read(T& data) { file->Read(data); }

   template<typename T>
   void Read(T* data, size_t count) { file->Read(data, count * sizeof(T)); }
   
   template<typename T>
   T ReadValue() { T a; file->Read(a); return a; }

   char* ReadString();

   SceneNode* GetNodeId(size_t index);
   void SetNodeId(size_t index, SceneNode* node);

   /// Return the graphics path of the parent folder of the file being loaded.
   /// The last slash will be ommitted, if present.  For example, if the
   /// requested filename was "Levels/House/House", this would return
   /// "Levels/House" even if the underlying filename was
   /// "Graphics/1024x768/Levels/House/House.fs"
   const char* GetParentPath() { return parentPath; }

   /// Return the number of texture nodes defined in the file.
   size_t GetNumberOfTextureNodes() { return numberOfTextureNodes; }
   /// Return the number of material nodes defined in the file.
   size_t GetNumberOfMaterialNodes() { return numberOfMaterialNodes; }

   /// Return the FileManager for loading the file and its dependencies.
   FileManager* FileManagerGet() { return fileManager; }

private:
   // Prevent construction/copying
   SceneLoadData(const SceneLoadData&);
   void operator=(const SceneLoadData&);

   /// Similar to FileManager::GraphicsPathFileLoad, except that it returns
   /// the decompressed data.
   void* LoadCompressedData(FileManager* fileManager, const char* filename, size_t* length,
      int alignment = FROG_MEM_ALIGN, HeapID heapID = HEAP_DEFAULT,
      HeapID heapTempID = HEAP_TEMP, char* concatenatedPath = NULL,
      size_t concatenatedPathSize = 0);

   /// Object used to parse decompressed data from the file.
   File* file;
   /// FileManager for loading the file and its dependencies.
   FileManager* fileManager;
   /// Buffer containing the decompressed data from the file.
   void* decompressedFileData;
   
   size_t numberOfSceneNodes;
   size_t numberOfTextureNodes;
   size_t numberOfMaterialNodes;
   SceneNode** sceneNodeList;

   /// See GetParentPath
   char parentPath[FROG_PATH_MAX];
};

//=============================================================================

} //namespace Webfoot

#endif //#ifndef __FROG__FROG3D__SCENELOADDATA_H__
