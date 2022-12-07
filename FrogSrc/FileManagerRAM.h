#ifndef __FROG__FILEMANAGERRAM_H__
#define __FROG__FILEMANAGERRAM_H__

#include "FrogMemory.h"
#include "File.h"
#include "FileManager.h"

namespace Webfoot {

struct FileEntry
{
   const char* filename;
   size_t length;
   void* data;
};

struct FileManagerRAMArchive
{
   int fileCount;
   FileEntry* fileEntries;
};

class FileManagerRAM : public FileManager
{
public:
   FileManagerRAM();
   virtual ~FileManagerRAM() {}
   
   void Init(FileManagerRAMArchive* _archive);

   /// Clean up the FileManager itself.
   virtual void Deinit();
   
   /// Open a new file object with the given options.
   /// Create the object in the specified heap.
   /// Do not use FileManager::WRITE in 'options' for this class.
   /// Return NULL if unsuccessful.
   virtual File* Open(const char* filename, int options = READ, HeapID heapID = HEAP_TEMP);
   /// Close the given file and free any memory allocated for it.
   /// Return true if successful.
   virtual bool Close(File* file);
   
   /// If the given path corresponds to an existing file, return true.
   /// Depending on the FileManager, it may or may not return true if it
   /// corresponds to a folder.  In this case, it does not return true for folders.
   virtual bool ExistsCheck(const char* path);   
   
   typedef FileManager Inherited;
   
protected:

   /// FileManagerRAMArchive that contains the files.
   FileManagerRAMArchive* archive;   

   friend class FileRAM;
};

//============================================================================== 

} //namespace Webfoot {

#endif //#ifndef __FROG__FILEMANAGERRAM_H__
