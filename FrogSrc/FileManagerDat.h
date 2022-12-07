#ifndef __FROG__FILEMANAGERDAT_H__
#define __FROG__FILEMANAGERDAT_H__

#include "FrogMemory.h"
#include "File.h"
#include "FileManager.h"

namespace Webfoot {

//Number used to identify a valid resource file
#define WT_IO_DAT_SIGNATURE       0xBABEFACE
#define WT_IO_DAT_SIGNATURE_CRYPT 0xCDCEDECD
#define WT_IO_DAT_XORAGE          0xAA

#if __PRAGMA_PACK__
#pragma pack (push, 1)
#endif

typedef struct __GCC_PACKED__ 
        {
        uint32 signature;
        uint32 offset;
        uint32 nbfiles;
        uchar  xorage;
        } WT_IO_DAT_SIGN;

typedef struct __GCC_PACKED__ 
        {
        uint32 offset;
        uint32 length;
        uint32 crc;
        uchar  xorage;
        } WT_IO_DAT_INFO;

typedef struct __GCC_PACKED__ 
        {
        char *shortname;
        char *longname;
        WT_IO_DAT_INFO *info;
        } WT_IO_DAT_TABLE;

#if __PRAGMA_PACK__
#pragma pack (pop)
#endif

class FileDat;

//==============================================================================

/// FileManagerDat abstracts the Wrappers resource file system.
/// Paths should use '/' as the separator rather than '\\'.
/// Be sure to read the comments on the member functions, because they are not
/// all supported on all platforms.  This will only work on platforms that
/// support packed structs and non-aligned numbers.
class FileManagerDat : public FileManager
{
public:
   FileManagerDat();
   virtual ~FileManagerDat() {}
   
   /// Initialize the manager to use the given resource file that exists in the 
   /// given parentFileManager.  Return true if successful.
   bool Init(FileManager* _parentFileManager, const char* _resourceFilename,
      HeapID heapID = HEAP_DEFAULT, const char* rootPath = NULL);
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

   FileManager* ParentFileManagerGet() { return parentFileManager; }
   
   typedef FileManager Inherited;
   
protected:
   /// FileManager that contains the resource file.
   FileManager* parentFileManager;
   /// Pointer to the resource table of content
   char* wt_io_resource_ptr;
   // Pointer to resource FAT
   WT_IO_DAT_TABLE *wt_io_resource_fat;
   /// Filename of the resource file.
   char resourceFilename[FROG_PATH_MAX+1];
   /// Path to treat as the root.
   char rootPath[FROG_PATH_MAX+1];

   friend class FileDat;
};

//============================================================================== 

} //namespace Webfoot {

#endif //#ifndef __FROG__FILEMANAGERDAT_H__
