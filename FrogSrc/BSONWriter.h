#ifndef __FROG__BSONWRITER_H__
#define __FROG__BSONWRITER_H__

#include "FrogMemory.h"
#include "FileManager.h"
#include "BSONValue.h"
#include "JSONValue.h"

namespace Webfoot {

//==============================================================================

/// BSONWriter is used to create BSON representations of data.  Do not use a
/// single BSONWriter instance to create more than one document at once.  For
/// example, if you need to save another document on a separate thread, use
/// separate instances of BSONWriter.
class BSONWriter
{
public:
   BSONWriter();

   /// Write the given tree of BSONValues to the given file.  Do not include
   /// the ".bson" extension in the filename.  Use the given string to indent.
   /// Return true if successful.
   bool Save(const char* filename, FileManager* fileManager, BSONValue* root, HeapID tempHeapID = HEAP_TEMP);
   /// Write the given tree of BSONValues to the given BSON file.  Return true
   /// if successful.
   bool Save(File* file, BSONValue* root);

   /// Write the given tree of JSONValues to the given file.  Do not include
   /// the ".bson" extension in the filename.  Use the given string to indent.
   /// Return true if successful.
   bool Save(const char* filename, FileManager* fileManager, JSONValue* root, HeapID tempHeapID = HEAP_TEMP);
   /// Write the given tree of JSONValues to the given BSON file.  Return true
   /// if successful.
   bool Save(File* file, JSONValue* root);

   /// Allocate and return a new buffer containing the given tree of BSONValues
   /// encoded in BSON.  Write the size of the resulting buffer to 'bufferSize'.
   /// Delete the result when you're done using FrogFree.  Return NULL if
   /// unsuccessful.
   void* WriteBuffer(BSONValue* bsonValue, size_t* bufferSize, HeapID heapID = HEAP_DEFAULT);
   /// Same as the BSONValue form of WriteBuffer, except it accepts a
   /// JSONValue* instead.
   void* WriteBuffer(JSONValue* jsonValue, size_t* bufferSize, HeapID heapID = HEAP_DEFAULT);

   /// Use FileManager::CompressedFileSave to store the given tree of BSONValues
   /// in the given filename.  Since this is expected to be used for files of
   /// various extensions, the extension must be provided in 'filename'.  Return
   /// true if successful.  Since the format may change, this should typically
   /// only be used for files which may be re-converted from other formats, like
   /// for an optimized bitmap format.  An optional 'contentVersion' can
   /// specified, which can be retreived when loading the compressed file.
   bool CompressedSave(const char* filename, FileManager* fileManager,
      BSONValue* root, uint32 contentVersion = 0, HeapID heapTempID = HEAP_TEMP);
   /// Same as the BSONValue form of CompressedSave, except it accepts a
   /// JSONValue* instead.
   bool CompressedSave(const char* filename, FileManager* fileManager,
      JSONValue* root, uint32 contentVersion = 0, HeapID heapTempID = HEAP_TEMP);

protected:
   /// Return the required size in bytes of the given tree of BSONValues once
   /// encoded in BSON.  Return -1 if unsuccessful.
   int EncodedSizeGet(BSONValue* bsonValue);
   /// Return the required size in bytes of the given tree of JSONValues once
   /// encoded in BSON.  Return -1 if unsuccessful.
   int EncodedSizeGet(JSONValue* jsonValue);

   /// Helper function to recursively print a tree of BSONValues to the file.
   /// Return true if writing should continue.
   bool ValueWrite(BSONValue* bsonValue);
   /// Write the BSON type number for a value of the given type to the file.
   void TypeWrite(BSONValue::ValueType type);
   /// Return the BSON type number for a value of the given type.
   uchar TypeEncodedNumberGet(BSONValue::ValueType type);

   /// Helper function to recursively print a tree of BSONValues to a buffer.
   bool ValueWrite(BSONValue* bsonValue, size_t& outputBufferPosition);
   /// Write the BSON type number for a value of the given type to the buffer.
   void TypeWrite(BSONValue::ValueType type, size_t& outputBufferPosition);

   /// Helper function to recursively print a tree of JSONValues to a buffer.
   bool ValueWrite(JSONValue* jsonValue, size_t& outputBufferPosition);
   /// Return the BSON value type that corresponds to the given JSON value
   /// type.
   BSONValue::ValueType BSONValueTypeGet(JSONValue::ValueType jsonValueType);

   /// Helper function to recursively print a tree of JSONValues.
   /// Return true if writing should continue.
   bool ValueWrite(JSONValue* jsonValue);
   /// Write the BSON type number for a value of the given type.
   void TypeWrite(JSONValue::ValueType type);

   /// Used for temporary allocations
   HeapID tempHeapID;
   /// File to which BSON is written.
   File* outputFile;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BSONWRITER_H__
