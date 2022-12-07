#ifndef __FROG__JSONWRITER_H__
#define __FROG__JSONWRITER_H__

#include "FrogMemory.h"
#include "FileManager.h"

namespace Webfoot {

class JSONValue;
class BSONValue;

//==============================================================================

/// JSONWriter is used to create JSON text representations of data.  Do not use a
/// single JSONWriter instance to create more than one document at once.  For
/// example, if you need to save another document on a separate thread, use
/// separate instances of JSONWriter.
class JSONWriter
{
public:
   JSONWriter();

   /// Return a JSON string representation of the given JSON data.  Use the
   /// given string to indent.  Delete the result when you're done using
   /// StringDelete.  Return NULL if unsuccessful.
   char* WriteString(JSONValue* root, const char* indentString = "   ",
      HeapID _heapID = HEAP_DEFAULT, HeapID _tempHeapID = HEAP_TEMP);

   /// Write the given tree of JSONValues to the given file.  Do not include
   /// the ".json" extension in the filename.  Use the given string to indent.
   /// Return true if successful.
   bool Save(const char* filename, FileManager* fileManager, JSONValue* root,
      const char* indentString = "   ", HeapID tempHeapID = HEAP_TEMP);
   /// Write the given tree of JSONValues to the given file.  Use the given
   /// string to indent.  Return true if successful.  'filename' is for debugging purposes.
   bool Save(File* file, JSONValue* root, const char* indentString = "   ",
      HeapID _tempHeapID = HEAP_TEMP, const char* filename = NULL);

   /// Write the given tree of BSONValues to the given file.  Do not include
   /// the ".json" extension in the filename.  Use the given string to indent.
   /// Return true if successful.
   bool Save(const char* filename, FileManager* fileManager, BSONValue* root,
      const char* indentString = "   ", HeapID tempHeapID = HEAP_TEMP);
   /// Write the given tree of BSONValues to the given file.  Use the given
   /// string to indent.  Return true if successful.  'filename' is for debugging purposes.
   bool Save(File* file, BSONValue* root, const char* indentString = "   ",
      HeapID _tempHeapID = HEAP_TEMP, const char* filename = NULL);

protected:

   /// Helper function to recursively print a tree of JSONValues.
   /// Return true if writing should continue.
   bool ValueWrite(JSONValue* jsonValue);
   /// Helper function to recursively print a tree of BSONValues.
   /// Return true if writing should continue.
   bool ValueWrite(BSONValue* bsonValue);

   /// Called by YAJL when the given non-null-terminated string needs to be added
   /// to the output file.
   static void YAJLFileWrite(void* context, const char* text, size_t textLength);
   /// Called by YAJL when the given non-null-terminated string needs to be added
   /// to the output buffer.
   static void YAJLBufferWrite(void* context, const char* text, size_t textLength);

   /// Called by YAJL when it needs to use malloc.
   static void* YALJMalloc(void* _heapID, size_t size);
   /// Called by YAJL when it needs to use free.
   static void YALJFree(void* _heapID, void* ptr);
   /// Called by YAJL when it needs to use realloc.
   static void* YALJRealloc(void* _heapID, void* ptr, size_t size);

   /// Heap used for potentially long-term allocations.
   HeapID heapID;
   /// Used for temporary allocations
   HeapID tempHeapID;
   /// File to which JSON is written.
   File* outputFile;
   /// Filename to which JSON is written. (for debugging purposes).
   const char* outputFilename;
   /// Flag used to indicate that something went wrong in YAJLFileWrite or YAJLBufferWrite
   bool writingFailed;
   /// When writing to a string buffer, this is that buffer.
   char* buffer;
   /// When writing to a string buffer, this is the number of bytes that have been
   /// used so far.
   size_t bufferSizeUsed;
   /// When writing to a string buffer, this is the size of the current buffer.
   size_t bufferSize;
   /// Handle to the JSON generator.
   void* _yajlGenerator;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__JSONWRITER_H__
