#ifndef __FROG__FILEMANAGERSTDIO_H__
#define __FROG__FILEMANAGERSTDIO_H__

#include "FrogMemory.h"
#include "File.h"
#include "FileManager.h"

namespace Webfoot {

//==============================================================================

/// FileManagerStdio abstracts the native stdio file system.
/// Paths should use '/' as the separator rather than '\\'.
/// Be sure to read the comments on the member functions, because they are not
/// all supported on all platforms.
class FileManagerStdio : public FileManager
{
public:
   FileManagerStdio();
   virtual ~FileManagerStdio() {}
   
   /// Initialize the manager to use the given root in the native file system.
   /// If no root path is specified, it will use the working directory.
   /// It doesn't matter whether '_rootPath' ends in a '/'.
   /// Return true if successful.
   bool Init(const char* _rootPath = NULL);
   
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
   /// corresponds to a folder.  In this case, it does return true for folders.
   virtual bool ExistsCheck(const char* path);
   /// Return true if the given path corresponds to a file in the file system,
   /// as opposed to a folder or nothing at all.  This will not necessarily be supported
   /// on all FileManagers.
   virtual bool FileCheck(const char* path);
   /// Return true if the given path corresponds to a folder in the file system,
   /// as opposed to a file or nothing at all.  This will not necessarily be supported
   /// on all FileManagers, especially archives.
   virtual bool FolderCheck(const char* path);
   
   /// Remove the given file from the file system.  Return true if successful.
   /// This is not supported on all implementations.
   virtual bool FileRemove(const char* path);
   /// Remove the given folder from the file system.  Return true if successful. 
   /// This assumes that the folder in question is empty.
   /// This is not supported on all implementations.
   virtual bool FolderRemove(const char* path);
   
   /// Change the name of the given file.  This is typically the same as moving it within the same volume.
   /// Return true if successful.  This is not supported on all implementations.
   virtual bool FileRename(const char* newPath, const char* oldPath);
   /// Change the name of the given folder.  This is typically the same as moving it within the same volume.
   /// Return true if successful.  This is not supported on all implementations.
   virtual bool FolderRename(const char* newPath, const char* oldPath);
   
   /// Copy the given file to the new location.  Return true if successful.
   /// This is not supported on all implementations.
   virtual bool FileCopy(const char* newPath, const char* oldPath, HeapID heapID = HEAP_TEMP);

   /// Create the given folder.  The parent of this folder should already exist
   /// if 'createParents' is false.  Return true if successful.  This is not
   /// supported on all implementations.
   virtual bool FolderCreate(const char* path, bool createParents = false);

   /// Return a table of the files and folders in the given folder.  This is not
   /// recursive.  This is not supported by all implementations.  This will return
   /// NULL if unsuccessful.  When you're done with the table returned by this
   /// function, pass it to FolderContentsTableFree. 
   virtual Table<const char*>* FolderContentsGet(const char* path,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP);

   /// Return a number for the time at which the given item was last modified.
   /// This is not supported by all implementations of FileManager. 
   virtual int64 ModificationTimeGet(const char* path);
   /// Set a number for the time at which the given item was last modified.
   /// This is not supported by all implementations of FileManager. 
   virtual bool ModificationTimeSet(const char* path, int64 modificationTime);

   /// Initialize the given FileManager for persistence data.
   /// This will make sure the tree of folders is prepared and set theFilesPersistence.
   /// Set 'rootPersistencePath' to the folder that stores all persistence data
   /// for the current user across all applications.  'parentFolderName' should
   /// be the name of the parent of the game-specific folder ("Webfoot").
   /// 'gameName' should be the name of the folder with the game-specific data.
   /// 'rootPersistencePath' should be an absolute path.  'theFilesPersistence'
   /// pointer will only be set if 'setTheFilesPersistence' is true.
   /// 'theFilesPersistenceLocal' pointer will only be set if
   /// 'setTheFilesPersistenceLocal' is true.  Return true if successful.
   static bool PersistenceFileManagerInit(FileManagerStdio* fileManagerPersistence, 
      const char* rootPersistencePath, const char* parentFolderName, const char* gameName,
      bool setTheFilesPersistence = true, bool setTheFilesPersistenceLocal = false);

   /// Set 'persistencePath' to the full path of the folder for persistence
   /// data for 'PersistenceFileManagerInit'.  Return true if successful.
   static bool PersistencePathBuild(char* persistencePath, size_t persistencePathSize, 
      const char* rootPersistencePath, const char* parentFolderName, const char* gameName);

   /// Based on command line parameters, build a chain of FileManagerStdios.
   /// Paths to include that are specified earlier will have priority over those
   /// which are specified later.
   /// For example, the following would return a FileManagerStdio that would
   /// look for files in "iOSPC" first, and then check in "PC" if didn't find
   /// a requested file.
   ///    GameD.exe --files "iOSPC" --files "PC"
   ///
   /// If none are specified, return NULL.  See FileManagerStdioChainDeinit for
   /// information on cleanup.
   static FileManagerStdio* FileManagerStdioChainInit(int argc, const char** argv, const char* argName = "--files");
   /// FileManagerStdioChainDeinit can be used to delete a chain of
   /// FileManagerStdios created by FileManagerStdioChainInit.  If you added
   /// anything to that chain, you may need to clean that up manually.  This
   /// should delete the given FileManagerStdio and all consecutive
   /// FileManagerStdios linked through FileManagerFallbackGet.
   static void FileManagerStdioChainDeinit(FileManagerStdio* fileManager);

   /// Sync changes to the underlying storage system.  This is not necessary
   /// on most platforms and FileManager implementations.
   virtual void Flush();

   typedef FileManager Inherited;
};

//============================================================================== 

} //namespace Webfoot {

#endif //#ifndef __FROG__FILEMANAGERSTDIO_H__
