#ifndef __FROG__JSONPARSER_H__
#define __FROG__JSONPARSER_H__

#include "FrogMemory.h"
#include "FileManager.h"
#include "Table.h"
#include "JSONValue.h"

namespace Webfoot {

//==============================================================================

/// JSONParser creates trees of JSONValues based on JSON text.  Do not use a
/// single JSONParser instance to parse more than one document at once.  For
/// example, if you need to load another document on a separate thread, use
/// separate instances of JSONParser.
class JSONParser
{
public:
   enum
   {
      /// Maximum lenght of a JSON object key, including the space for the
      /// null-terminator.
      KEY_LENGTH_MAX = 256
   };

   /// Parse the given string of JSON.  Long-term allocations will be made with
   /// '_heapID', and short-term allocations will be made with '_tempHeapID'.
   /// By default, all allocations use the temporary heap.  Deinit and delete
   /// the returned JSONValue when you are done with it and its descendants.
   JSONValue* ParseString(const char* jsonString, HeapID _heapID = HEAP_DEFAULT,
      HeapID _tempHeapID = HEAP_TEMP, bool showErrors = true);

   /// Load the given file from the given manager and return it parsed into a
   /// tree of JSONValues.  Do not include the ".json" extension in the
   /// filename.  Long-term allocations will be made with '_heapID', and
   /// short-term allocations will be made with '_tempHeapID'.  By default,
   /// all allocations use the temporary heap.  Deinit and delete the
   /// returned JSONValue when you are done with it and its descendants.
   JSONValue* Load(const char* filename, FileManager* fileManager = theFiles,
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);
   /// Parse the given JSON file, beginning at the current position.  Long-term
   /// allocations will be made with '_heapID', and short-term allocations will
   /// be made with '_tempHeapID'.  By default, all allocations use the
   /// temporary heap.  Deinit and delete the returned JSONValue when you are
   /// done with it and its descendants.  'filename' is for debugging purposes.
   JSONValue* Load(File* file, HeapID _heapID = HEAP_DEFAULT,
      HeapID _tempHeapID = HEAP_TEMP, const char* filename = NULL);

   /// Return true if a JSON file exists with the given filename in the given
   /// FileManager.  'filename' should exclude the extension.
   bool ExistsCheck(const char* filename, FileManager* fileManager);

   /// This is similar to the normal filename-based version of Load, except that
   /// it looks for the file in the graphics paths.
   /// If 'concatenatedPath' is specified and the path does exist, the result of
   /// concatenating the relevant graphics path, the requested path, and the
   /// file extension will be written to 'concatenatedPath'.  If the file is
   /// opened, but something goes wrong while writing to 'concatenatedPath',
   /// the file will be closed internally and NULL will be returned.  
   JSONValue* GraphicsPathLoad(const char* filename, FileManager* fileManager = theFiles,
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

   /// This is similar to 'Load' except that it recursively loads and merges
   /// JSON files.  If 'parentKey' is "Inherited" and a file specifies a string
   /// entry for "Inherited", that string is used as the parent filename.  The
   /// parent is loaded and merged at a lower priority.  If "Inherited"
   /// specifies an array of strings, those strings are the parent filenames,
   /// which are processed recusively and merged, at increasing priority.
   JSONValue* LoadRecursive(const char* filename, FileManager* fileManager = theFiles,
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);
   
protected:
   /// Called by YAJL when it needs to use malloc.
   static void* YALJMalloc(void* _heapID, size_t size);
   /// Called by YAJL when it needs to use free.
   static void YALJFree(void* _heapID, void* ptr);
   /// Called by YAJL when it needs to use realloc.
   static void* YALJRealloc(void* _heapID, void* ptr, size_t size);
   
   /// Called by YAJL when it reaches a null value.
   static int YAJLNullCallback(void* context);
   /// Called by YAJL when it reaches a boolean value.
   static int YAJLBooleanCallback(void* context, int value);
   /// Called by YAJL when it reaches an integer value.
   static int YAJLIntegerCallback(void* context, long long value);
   /// Called by YAJL when it reaches a double value.
   static int YAJLDoubleCallback(void* context, double value);
   /// Called by YAJL when it reaches a string value.  The string is not null-terminated.
   static int YAJLStringCallback(void* context, const unsigned char* value, size_t valueLength);
   /// Called by YAJL when it reaches the beginning of an object.
   static int YAJLStartObjectCallback(void* context);
   /// Called by YAJL when it reaches a key within an object.  The string is not null-terminated.
   static int YAJLStartObjectKeyCallback(void* context, const unsigned char* key, size_t keyLength);
   /// Called by YAJL when it reaches the end of an object.
   static int YAJLEndObjectCallback(void* context);
   /// Called by YAJL when it reaches the beginning of an array.
   static int YAJLStartArrayCallback(void* context);
   /// Called by YAJL when it reaches the end of an array.
   static int YAJLEndArrayCallback(void* context);

   /// Used to create the JSONValue instances.
   HeapID heapID;
   /// Used for temporary allocations
   HeapID tempHeapID;
   /// Temporary copy of the key to use for the next object entry.
   char nextKey[KEY_LENGTH_MAX];
   /// Used to keep track of the current position in the tree of JSONValues
   /// being built.
   Table<JSONValue*> valueStack;
   /// Root of the tree of JSONValues.
   JSONValue* root;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__JSONPARSER_H__
