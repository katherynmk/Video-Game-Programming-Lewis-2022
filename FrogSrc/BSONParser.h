#ifndef __FROG__BSONPARSER_H__
#define __FROG__BSONPARSER_H__

#include "FrogMemory.h"
#include "FileManager.h"
#include "Table.h"
#include "BSONIterator.h"

namespace Webfoot {

class BSONValue;

//==============================================================================

/// BSONParser creates trees of BSONValues based on BSON data.  Do not use a
/// single BSONParser instance to parse more than one document at
/// once.  For example, if you need to load another document on a separate
/// thread, use separate instances of BSONParser.
class BSONParser
{
public:
   /// Load the given file from the given manager and return it parsed into a
   /// tree of BSONValues.  Do not include the ".bson" extension in the
   /// filename.  Long-term allocations will be made with '_heapID', and
   /// short-term allocations will be made with '_tempHeapID'.  By default,
   /// all allocations use the temporary heap.  Deinit and delete the
   /// returned BSONValue when you are done with it and its descendants.
   BSONValue* Load(const char* filename, FileManager* fileManager = theFiles,
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   /// Load the given BSON data into a tree of BSONValues.  Long-term
   /// allocations will be made with '_heapID'.  Deinit and delete the returned
   /// BSONValue when you are done with it and its descendants.  'filename' is
   /// for debugging purposes.
   BSONValue* Load(BSONIterator iterator, HeapID heapID = HEAP_DEFAULT,
      const char* filename = NULL);

   /// Return true if a BSON file exists with the given filename in the given
   /// FileManager.  'filename' should exclude the extension.
   bool ExistsCheck(const char* filename, FileManager* fileManager);

   /// This is similar to the normal filename-based version of Load, except that
   /// it looks for the file in the graphics paths.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path, the requested path, and the
   /// file extension will be written to 'concatenatedPath'.  If the file is
   /// opened, but something goes wrong while writing to 'concatenatedPath',
   /// the file will be closed internally and NULL will be returned.  
   BSONValue* GraphicsPathLoad(const char* filename, FileManager* fileManager = theFiles,
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP,
      char* concatenatedPath = NULL, size_t concatenatedPathSize = 0);
      
   /// Return true if the given file exists in one of the graphics paths.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path, the requested path, and the
   /// file extension will be written to 'concatenatedPath'.  If the path
   /// exists, but something goes wrong while writing to 'concatenatedPath',
   /// return false.
   bool GraphicsPathExistsCheck(const char* filename, FileManager* fileManager = theFiles,
      char* concatenatedPath = NULL, size_t concatenatedPathSize = 0);

   /// This is similar to the normal Load except you need to include the file
   /// extension and the file data is loaded with
   /// FileManager::CompressedFileLoad.  The 'contentVersion' that was
   /// embedded in the file can optionally be retreived by setting the
   /// 'contentVersion' pointer to an address to receive it.
   BSONValue* CompressedLoad(const char* filename, FileManager* fileManager = theFiles,
      uint32* contentVersion = NULL, HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);

protected:
   /// Used to create the BSONValue instances.
   HeapID heapID;
   /// Used for temporary allocations
   HeapID tempHeapID;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BSONPARSER_H__
