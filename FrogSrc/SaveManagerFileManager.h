#ifndef __FROG__SAVEMANAGERFILEMANAGER_H__
#define __FROG__SAVEMANAGERFILEMANAGER_H__

#include "FrogMemory.h"
#include "SaveManager.h"

namespace Webfoot {

//==============================================================================

/// SaveManagerFileManager is a SaveManager implementation that relies on a
/// FileManager to save and load data.
/// When providing filenames of save files, do not include the extension.
class SaveManagerFileManager : public SaveManager
{
public:
   SaveManagerFileManager();

   virtual ~SaveManagerFileManager() {}

   void Init(FileManager* _fileManager = theFilesPersistence, HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   virtual void Deinit();
   
   /// Attempt to save the given data to the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.  'usedSize' should be exactly the amount of space that is
   /// actually used in 'buffer'.  If the given slot has a specific size, 'usedSize' must be
   /// less than or equal to that size.  'buffer' and 'usedSize' must include
   /// space at the beginning for the SaveSlotHeader.  The header will be filled
   /// out automatically based on the parameters.
   virtual ErrorCode Save(void* buffer, size_t usedSize, const char* filename, int slotIndex = -1);
   /// Attempt to load data from the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.  'bufferSize' must be large enough to hold all the data
   /// from the given slot.  'buffer' and 'bufferSize' must include space at the
   /// beginning for the SaveSlotHeader.  If successful and 'usedSize' is
   /// specified, it will be set to the value that would have been used in the
   /// corresponding Save call.
   virtual ErrorCode Load(void* buffer, size_t bufferSize, size_t* usedSize, const char* filename, int slotIndex = -1);     
   
   /// Attempt to save the given variables to the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.
   virtual ErrorCode Save(const char* variableList, const char* filename, int slotIndex = -1);
   /// Attempt to load the given variables from the given slot of the given file.
   /// If the file consists of a single dynamically sized slot, use -1 for
   /// 'slotIndex'.
   virtual ErrorCode Load(const char* variableList, const char* filename, int slotIndex = -1);

   /// Remove the given save file.  Return SUCCESS if the file was either deleted
   /// or not found.
   virtual ErrorCode Erase(const char* filename);

   typedef SaveManager Inherited;
   
protected:
   /// Set 'fullFilename' to the concatenation of 'filename' and the appropriate extension.
   /// Return true if successful.
   virtual bool FullFilenameBuild(char* fullFilename, const char* filename, size_t fullFilenameBufferSize);

   FileManager* fileManager;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__SAVEMANAGERFILEMANAGER_H__