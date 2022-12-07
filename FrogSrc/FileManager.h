#ifndef __FROG__FILEMANAGER_H__
#define __FROG__FILEMANAGER_H__

#include "FrogMemory.h"
#include "Allocator.h"
#include "Table.h"

namespace Webfoot {

/// Use this when creating string buffers for filenames.
/// The true maximum will vary by FileManager implementation.
#define FROG_PATH_MAX 260

class File;

//==============================================================================

/// FileManager is an interface for abstracting different types of file systems.
/// This includes volumes on optical discs, hard discs, NAND memory,
/// and archives that exist within other file systems.  All files are opened
/// in binary mode.  Large file support should be available where practical.  
/// Be sure to read the comments on the member functions, because they are not
/// all supported on all implementations.
class FileManager
{
public:
   FileManager();
   virtual ~FileManager() {}

   virtual void Deinit();
   
   /// Bit flags for the 'options' parameter of the 'Open' method.
   enum 
   {
      /// Allow reading from the file.
      READ = 1,
      /// Allow writing to the file.
      WRITE = 2,
      /// Append to the end of the file.
      APPEND = 4
   };
   
   /// Open a new file object with the given options.
   /// Create the object in the specified heap.
   /// Return NULL if unsuccessful.
   virtual File* Open(const char* filename, int options = READ, HeapID heapID = HEAP_TEMP) = 0;
   /// Close the given file and free any memory allocated for it.
   /// Return true if successful.
   virtual bool Close(File* file) = 0;
   
   /// Load an entire file to a new allocation in the given heap.
   /// The length of the file will be written to 'length' if successful.
   /// Free anything obtained from this function using FileUnload.
   /// Memory will be aligned to 'alignment' at minimum, but individual
   /// implementations may use a higher alignment if necessary.
   /// Return NULL if unsuccessful.
   virtual void* FileLoad(const char* filename, size_t* length,
      int alignment = FROG_MEM_ALIGN, HeapID heapID = HEAP_DEFAULT,
      HeapID heapTempID = HEAP_TEMP)
   {
      return FileLoadHelper(filename, length, NULL, alignment, heapID, heapTempID);
   }
   /// Load an entire file into a new allocation from the given allocator.
   /// It is up to the application programmer to make sure the allocator will
   /// return memory of an appropriate alignment for the platform and intended use.
   /// Free the memory using the same allocator.  Do not use FileUnload with this form.
   virtual void* FileLoad(const char* filename, size_t* length, Allocator* allocator,
      HeapID heapTempID = HEAP_TEMP)
   {
      return FileLoadHelper(filename, length, allocator, FROG_MEM_ALIGN, HEAP_DEFAULT, heapTempID);
   }
   /// Free data obtained with the heap/alignment form of FileLoad.
   /// For the allocator form of FileLoad, use the same allocator to free the memory.
   virtual void FileUnload(void* data) { FrogFree(data); }

   /// Load an entire file as a null-terminated UTF-8 string to a new allocation in the given heap.
   /// The length of the string, excluding the null-terminator, will be written to 'length' if successful.
   /// Free anything obtained from this function using TextFileUnload.
   /// Memory will be aligned to 'alignment' at minimum, but individual
   /// implementations may use a higher alignment if necessary.
   /// Return NULL if unsuccessful.
   virtual char* TextFileLoad(const char* filename, size_t* length = NULL,
      int alignment = FROG_MEM_ALIGN, HeapID heapID = HEAP_DEFAULT,
      HeapID heapTempID = HEAP_TEMP);
   /// Free data obtained with TextFileUnload.
   virtual void TextFileUnload(const char* data) { FrogFree((void*)data); }

   /// Load a file that was compressed with CompressedFileSave.  Since the
   /// format may change, this should typically only be used for files which
   /// may be re-converted from other formats, like for an optimized bitmap
   /// format.  The 'contentVersion' that was embedded in the file can
   /// optionally be retreived by setting the 'contentVersion' pointer to
   /// an address to receive it.
   void* CompressedFileLoad(const char* filename, size_t* length,
      uint32* contentVersion = NULL, int alignment = FROG_MEM_ALIGN,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP);
   /// Free data obtained with CompressedFileLoad.
   void CompressedFileUnload(void* data) { FrogFree((void*)data); }
   /// Compress and save the given data to a file.  Return true if successful.
   /// Since the format may change, this should typically only be used for
   /// files which may be re-converted from other formats, like for an
   /// optimized bitmap format.  An optional 'contentVersion' can be
   /// specified, which can be retreived when loading the compressed file.
   bool CompressedFileSave(const char* filename, const void* data,
      size_t dataSize, uint32 contentVersion = 0, HeapID heapTempID = HEAP_TEMP);

   /// If the given path corresponds to an existing file, return true.
   /// Depending on the FileManager, it may or may not return true if it
   /// corresponds to a folder.
   virtual bool ExistsCheck(const char* path) = 0;
   /// Return true if the given path corresponds to a file in the file system,
   /// as opposed to a folder or nothing at all.  This will not necessarily be supported
   /// on all FileManagers.
   virtual bool FileCheck(const char* /*path*/) { return false; }
   /// Return true if the given path corresponds to a folder in the file system,
   /// as opposed to a file or nothing at all.  This will not necessarily be supported
   /// on all FileManagers, especially archives.
   virtual bool FolderCheck(const char* /*path*/) { return false; }
   
   /// Remove the given file from the file system.  Return true if successful.
   /// This is not supported on all implementations.
   virtual bool FileRemove(const char* /*path*/) { return false; }
   /// Remove the given folder from the file system.  Return true if successful.
   /// This assumes that the folder in question is empty.
   /// This is not supported on all implementations.
   virtual bool FolderRemove(const char* /*path*/) { return false; }
   
   /// Change the name of the given file.  This is typically the same as moving it within the same volume.
   /// Return true if successful.  This is not supported on all implementations.
   virtual bool FileRename(const char* /*newPath*/, const char* /*oldPath*/) { return false; }
   /// Change the name of the given folder.  This is typically the same as moving it within the same volume.
   /// Return true if successful.  This is not supported on all implementations.
   virtual bool FolderRename(const char* /*newPath*/, const char* /*oldPath*/) { return false; }
   
   /// Copy the given file to the new location.  
   /// Make temporary allocations from the given heap.
   /// Return true if successful.  This is not supported on all implementations.
   virtual bool FileCopy(const char* /*newPath*/, const char* /*oldPath*/, HeapID /*heapID*/ = HEAP_TEMP) { return false; }

   /// Create the given folder.  The parent of this folder should already exist
   /// if 'createParents' is false.  Return true if successful.  This is not
   /// supported on all implementations.
   virtual bool FolderCreate(const char* path, bool createParents = false) { (void)path; (void)createParents; return false; }

   /// Return a table of the files and folders in the given folder.  This is not
   /// recursive.  This is not supported by all implementations.  This will return
   /// NULL if unsuccessful.  When you're done with the table returned by this
   /// function, pass it to FolderContentsTableFree.
   virtual Table<const char*>* FolderContentsGet(const char* path,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP) { (void)path; (void)heapID; (void)heapTempID; return NULL; }
   /// Cleans up a Table returned by FolderContentsGet.
   void FolderContentsTableFree(Table<const char*>* contents);

   /// Return a number for the time at which the given item was last modified.
   /// This is not supported by all implementations of FileManager. 
   virtual int64 ModificationTimeGet(const char* path) { (void)path; return 0; }
   /// Set a number for the time at which the given item was last modified.
   /// This is not supported by all implementations of FileManager.  Return
   /// true if successful.
   virtual bool ModificationTimeSet(const char* path, int64 modificationTime) { (void)path; (void)modificationTime; return false; }

   /// If unable to find a file in the file system, try tooking for it here.
   /// This may not be supported by all implementations of FileManager.
   void FileManagerFallbackSet(FileManager* _fileManagerFallback) { fileManagerFallback = _fileManagerFallback; }
   /// Return the current fallback FileManager, if any.  This may not be
   /// supported by all implementations of FileManager.
   FileManager* FileManagerFallbackGet() { return fileManagerFallback; }

   /// Normally, return true if the given path exists in one of the graphics paths.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path and the requested path will be
   /// written to 'concatenatedPath'.  If the path exists, but something goes
   /// wrong while writing to 'concatenatedPath', return false. 
   bool GraphicsPathExistsCheck(const char* path,
      char* concatenatedPath = NULL, size_t concatenatedPathSize = 0);
   
   /// If the given file exists in one of the graphics paths, open it and return it.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path and the requested path will be
   /// written to 'concatenatedPath'.  If the file is opened, but something goes
   /// wrong while writing to 'concatenatedPath', the file will be closed
   /// internally and NULL will be returned. 
   File* GraphicsPathOpen(const char* filename, int options = READ,
      HeapID heapID = HEAP_TEMP, char* concatenatedPath = NULL,
      size_t concatenatedPathSize = 0);

   /// If the given file exists in one of the graphics paths, load the entire
   /// file into a new allocation from the given heap.
   /// The length of the file will be written to 'length' if successful.
   /// Free anything obtained from this function using FileUnload.
   /// Memory will be aligned to 'alignment' at minimum, but individual
   /// implementations may use a higher alignment if necessary.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path and the requested path will be
   /// written to 'concatenatedPath'.  If the file is opened, but something goes
   /// wrong while writing to 'concatenatedPath', the file will be closed
   /// internally and NULL will be returned.
   virtual void* GraphicsPathFileLoad(const char* filename, size_t* length,
      int alignment = FROG_MEM_ALIGN, HeapID heapID = HEAP_DEFAULT,
      HeapID heapTempID = HEAP_TEMP, char* concatenatedPath = NULL,
      size_t concatenatedPathSize = 0);

   /// If the given file exists in one of the graphics paths, load the entire
   /// file into a new allocation from the given allocator.
   /// It is up to the application programmer to make sure the allocator will
   /// return memory of an appropriate alignment for the platform and intended use.
   /// Free the memory using the same allocator.  Do not use FileUnload with this form.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path and the requested path will be
   /// written to 'concatenatedPath'.  If the file is opened, but something goes
   /// wrong while writing to 'concatenatedPath', the file will be closed
   /// internally and NULL will be returned.
   virtual void* GraphicsPathFileLoad(const char* filename, size_t* length,
      Allocator* allocator, HeapID heapTempID = HEAP_TEMP, char* concatenatedPath = NULL,
      size_t concatenatedPathSize = 0);

   /// This is similar to GraphicsPathFileLoad except that the file is expected
   /// to be using the same format as CompressedFileLoad.  Use
   /// CompressedFileUnload when finished.  The 'contentVersion' that was
   /// embedded in the file can optionally be retreived by setting the
   /// 'contentVersion' pointer to an address to receive it.
   void* GraphicsPathCompressedFileLoad(const char* filename, size_t* length,
      uint32* contentVersion = NULL, int alignment = FROG_MEM_ALIGN,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP,
      char* concatenatedPath = NULL, size_t concatenatedPathSize = 0);

   /// Similar to 'FolderContentsGet' but for files and folders in the graphics
   /// paths.
   virtual Table<const char*>* GraphicsPathFolderContentsGet(const char* path,
      HeapID heapID = HEAP_DEFAULT, HeapID heapTempID = HEAP_TEMP);

   /// Sets the root path for the filesystem.  This is not supported by all
   /// FileManager implementations.
   virtual bool RootPathSet(const char* _rootPath);

   /// Sync changes to the underlying storage system.  This is not necessary
   /// on most platforms and FileManager implementations.
   virtual void Flush() {}

protected:
   /// Given an input path, write a version of the path to 'outputBuffer' that takes
   /// the rootPath into account.  'bufferSize' is the size of 'outputBuffer'
   /// including the space for the NULL terminator.  Return true if successful.
   bool RootPathApply(char* outputBuffer, size_t bufferSize, const char* inputPath);

   /// If unable to find a file in the file system, try tooking for it here.
   /// This may not be supported by all implementations of FileManager.
   FileManager* fileManagerFallback;
   /// Path to treat as the root.  This is not used by all FileManager
   /// implementations.
   char rootPath[FROG_PATH_MAX+1];

private:
   /// Helper function to FileLoad.  Use 'allocator' if provided.  Otherwise use
   /// 'alignment' and 'heapID'.  Allocate at least 'extraSize' bytes more
   /// than is needed to hold the entire file.  The output 'length' does
   /// not include the 'extraSize'.
   void* FileLoadHelper(const char* filename, size_t* length, Allocator* allocator,
      int alignment, HeapID heapID, size_t extraSize = 0, HeapID tempHeapID = HEAP_TEMP);
};

//==============================================================================

/// Pointer to the default file manager for loading resources.
/// This must be set by the application before any usage attempts are made.
extern FileManager* theFiles;

/// Pointer to the default file manager for persistence data.
/// This must be set by the application before any usage attempts are made.
/// If this file system cannot be used, this pointer will be NULL.
extern FileManager* theFilesPersistence;

/// Pointer to the default file manager for persistence data which is ideally
/// not synced to other computers (like video settings).  Not all platforms
/// have a separate location for this, so it may be the same as
/// theFilesPersistence.  Not all games need this, so it may be NULL.  This
/// must be set by the application before any usage attempts are made.  If
/// this file system cannot be used, this pointer will be NULL.
extern FileManager* theFilesPersistenceLocal;

/// Pointer to the optional file manager for caching optimized versions of files
/// from the main file system.  If used, this must be set by the application
/// before any usage attempts are made.  This is not necessarily supported on
/// all platforms or for all file types.
extern FileManager* theFilesCache;

} //namespace Webfoot {

#endif //#ifndef __FROG__FILEMANAGER_H__
