#ifndef __FROG__BSONITERATOR_H__
#define __FROG__BSONITERATOR_H__

#include "FrogMemory.h"
#include "Point2.h"
#include "Point3.h"
#include "Box2.h"
#include "Box3.h"
#include "Color.h"
#include "Quaternion.h"

namespace Webfoot {

class JSONValue;

//==============================================================================

/// Extension to use on BSON files.
#define BSON_FILE_EXTENSION "bson"

/// Underlying BSON element type number for a 64-bit float.
#define BSON_ELEMENT_TYPE_RAW_DOUBLE 0x01
/// Underlying BSON element type number for a string.
#define BSON_ELEMENT_TYPE_RAW_STRING 0x02
/// Underlying BSON element type number for a string-indexed collection.
#define BSON_ELEMENT_TYPE_RAW_OBJECT 0x03
/// Underlying BSON element type number for an integer-indexed collection.
#define BSON_ELEMENT_TYPE_RAW_ARRAY 0x04
/// Underlying BSON element type number for a block of binary data.
#define BSON_ELEMENT_TYPE_RAW_BINARY 0x05
/// Underlying BSON element type number for a bool.
#define BSON_ELEMENT_TYPE_RAW_BOOLEAN 0x08
/// Underlying BSON element type number for a null.
#define BSON_ELEMENT_TYPE_RAW_NULL 0x0A
/// Underlying BSON element type number for a signed 32-bit integer.
#define BSON_ELEMENT_TYPE_RAW_INT32 0x10
/// Underlying BSON element type number for a signed 64-bit integer.
#define BSON_ELEMENT_TYPE_RAW_INT64 0x12

//==============================================================================

/// Used to navigate a static buffer of BSON data.
class BSONIterator
{
public:
   BSONIterator() { bufferPosition = NULL; isRoot = false; }
   void Init(const void* _bufferPosition, bool _isRoot, int _index = 0) { bufferPosition = _bufferPosition; isRoot = _isRoot; index = _index; }

   /// Go to the next item in the sequence.
   void Next();
   /// Return true if there is another item in the sequence after this position.
   bool NextCheck();
   /// Return a reference to the key at the current position.
   const char* Key();
   /// Assuming this is iterating over an Array, return the index of the current
   /// element.
   int Index() { return index; }
   /// Return true if the iterator is within the sequence.  Return false if it
   /// is past the end.
   bool WithinCheck();
   /// Return a copy of this iterator.
   BSONIterator Clone();

   /// Assuming the current element is an Array, start a new iterator for
   /// iterating over that Array.
   BSONIterator ArrayBegin();
   /// Assuming the current element is an Object, start a new iterator for
   /// iterating over that Object.
   BSONIterator ObjectBegin();

   /// If the current element is an Array or an Object, return the number
   /// of elements within that collection.  If the element is binary data,
   /// return the size of the data in bytes.
   int SizeGet();

   /// Assuming the current element is an array, return the value at the given
   /// index.  If you call this method with an index that is not valid, it will
   /// return a reference to a shared dummy BSONIterator of TYPE_NULL.  If you
   /// need to know whether a given entry is actually defined or what type it
   /// is, use NullCheck, NumberCheck, etc.
   BSONIterator Get(int _index);
   /// Assuming this is an object, return the value with the given key.  If you
   /// call this method with a key that is not valid, it will return a
   /// reference to a shared dummy BSONIterator of TYPE_NULL.  If you need to
   /// know whether a given entry is actually defined or what type it is, use
   /// NullCheck, NumberCheck, etc.
   BSONIterator Get(const char* key);

   /// Convert the object to a bool.  If the underlying value is actually a
   /// non-zero number, it will return true.  Defaults to false.
   operator bool();
   /// Convert the object to an int.  If the underlying value is actually a
   /// boolean, it will return 1 for true and 0 for false.  Defaults to 0.
   operator int();
   /// Convert the object to an unsigned int.  If the underlying value is actually a
   /// boolean, it will return 1 for true and 0 for false.  Defaults to 0.
   operator unsigned int();
   /// Convert the object to an int64.  If the underlying value is actually a
   /// boolean, it will return 1 for true and 0 for false.  Defaults to 0.
   operator int64();
   /// Convert the object to a float.  If the underlying value is actually a
   /// boolean, it will return 1.0f for true and 0.0f for false.  Defaults to 0.0f.
   operator float();
   /// Convert the object to a double.  If the underlying value is actually a
   /// boolean, it will return 1.0 for true and 0.0 for false.  Defaults to 0.0.
   operator double();
   /// Convert the object to a string.  Defaults to NULL.
   operator const char*();
   /// Assuming this element is binary data, return a pointer to that data.
   /// Defaults to NULL.
   operator const void*();
   /// Convert the object to a Point2I.  Defaults to all zeros.
   operator Point2I();
   /// Convert the object to a Point2F.  Defaults to all zeros.
   operator Point2F();
   /// Convert the object to a Point3F.  Defaults to all zeros.
   operator Point3F();
   /// Convert the object to a Box2F.  Defaults to all zeros.
   operator Box2F();
   /// Convert the object to a Box3F.  Defaults to all zeros.
   operator Box3F();
   /// Convert the object to a 32-bit color.  Defaults to black.
   operator ColorRGBA8();
   /// Convert the object to a ColorRGBA32F.  Defaults to black.
   operator ColorRGBA32F();
   /// Convert the object to a Quaternion.  Defaults to identity.
   operator Quaternion();

   /// Return true if this is not a defined value.
   bool NullCheck();
   /// Assuming this is a BSON array, return true if the value at the given
   /// index is not defined.
   bool NullCheck(int index) { return Get(index).NullCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key
   /// is not defined.
   bool NullCheck(const char* key) { return Get(key).NullCheck(); }

   /// Return true if this represents a boolean.
   bool BooleanCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a boolean.
   bool BooleanCheck(int index) { return Get(index).BooleanCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a boolean.
   bool BooleanCheck(const char* key) { return Get(key).BooleanCheck(); }

   /// Return true if this represents a double.
   bool DoubleCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a double.
   bool DoubleCheck(int index) { return Get(index).DoubleCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a double.
   bool DoubleCheck(const char* key) { return Get(key).DoubleCheck(); }

   /// Return true if this represents an int32.
   bool Int32Check();
   /// Assuming this is a BSON array, return true if the value at the given index represents an int32.
   bool Int32Check(int index) { return Get(index).Int32Check(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents an int32.
   bool Int32Check(const char* key) { return Get(key).Int32Check(); }

   /// Return true if this represents an int64.
   bool Int64Check();
   /// Assuming this is a BSON array, return true if the value at the given index represents an int64.
   bool Int64Check(int index) { return Get(index).Int64Check(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents an int64.
   bool Int64Check(const char* key) { return Get(key).Int64Check(); }

   /// Return true if this represents a number.
   bool NumberCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a number.
   bool NumberCheck(int index) { return Get(index).NumberCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a number.
   bool NumberCheck(const char* key) { return Get(key).NumberCheck(); }

   /// Return true if this represents a string.
   bool StringCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a string.
   bool StringCheck(int index) { return Get(index).StringCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a string.
   bool StringCheck(const char* key) { return Get(key).StringCheck(); }

   /// Return true if this represents a block of binary data.
   bool BinaryCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a block of binary data.
   bool BinaryCheck(int index) { return Get(index).BinaryCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a block of binary data.
   bool BinaryCheck(const char* key) { return Get(key).BinaryCheck(); }

   /// Return true if this can be interpreted as a Point2I.
   bool Point2ICheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Point2I.
   bool Point2ICheck(int index) { return Get(index).Point2ICheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Point2F.
   bool Point2ICheck(const char* key) { return Get(key).Point2ICheck(); }

   /// Return true if this can be interpreted as a Point2F.
   bool Point2FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Point2F.
   bool Point2FCheck(int index) { return Get(index).Point2FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Point2F.
   bool Point2FCheck(const char* key) { return Get(key).Point2FCheck(); }

   /// Return true if this can be interpreted as a Point3F.
   bool Point3FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Point3F.
   bool Point3FCheck(int index) { return Get(index).Point3FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Point3F.
   bool Point3FCheck(const char* key) { return Get(key).Point3FCheck(); }

   /// Return true if this can be interpreted as a Box2F.
   bool Box2FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Box2F.
   bool Box2FCheck(int index) { return Get(index).Box2FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Box2F.
   bool Box2FCheck(const char* key) { return Get(key).Box2FCheck(); }

   /// Return true if this can be interpreted as a Box3F.
   bool Box3FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Box3F.
   bool Box3FCheck(int index) { return Get(index).Box3FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Box3F.
   bool Box3FCheck(const char* key) { return Get(key).Box3FCheck(); }

   /// Return true if this can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check(int index) { return Get(index).ColorRGBA8Check(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check(const char* key) { return Get(key).ColorRGBA8Check(); }

   /// Return true if this can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck(int index) { return Get(index).ColorRGBA32FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck(const char* key) { return Get(key).ColorRGBA32FCheck(); }

   /// Return true if this can be interpreted as a Quaternion.
   bool QuaternionCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Quaternion.
   bool QuaternionCheck(int index) { return Get(index).QuaternionCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Quaternion.
   bool QuaternionCheck(const char* key) { return Get(key).QuaternionCheck(); }

   /// Return true if this represents a BSON array.
   bool ArrayCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a BSON array.
   bool ArrayCheck(int index) { return Get(index).ArrayCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a BSON array.
   bool ArrayCheck(const char* key) { return Get(key).ArrayCheck(); }

   /// Return true if this represents a BSON object.
   bool ObjectCheck();
   /// Assuming this is a BSON array, return true if the value at the given index represents a BSON object.
   bool ObjectCheck(int index) { return Get(index).ObjectCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a BSON object.
   bool ObjectCheck(const char* key) { return Get(key).ObjectCheck(); }

   /// Create a JSONValue tree based on the data at the current position.
   JSONValue* JSONClone(HeapID heapID = HEAP_DEFAULT, HeapID tempHeapID = HEAP_TEMP);

   /// Returned by the Get methods if an entry isn't defined.
   static BSONIterator dummyIterator;

protected:
   /// Return the BSON element type number for the current element.
   uchar ElementTypeRawGet();
   /// Helper function for beginning an iteration of an Array or Object.
   BSONIterator BeginHelper();
   /// Helper function which returns a pointer to the memory after the name of
   /// the current element.
   const void* AfterNameGet();
   /// Assuming this element is a double, return its value.
   double DoubleRawGet();
   /// Assuming this element is a string, return a pointer to it.
   const char* StringRawGet();
   /// Assuming this element is a bool, return its value.
   bool BoolRawGet();
   /// Assuming this element is an int32, return its value.
   int32 Int32RawGet();
   /// Assuming this element is an int64, return its value.
   int64 Int64RawGet();
   /// Helper function for determining the position of the next element in the
   /// current collection.
   const void* BufferPositionNextGet();

   /// Pointer to the beginning of the current element in the block of BSON
   /// data.
   const void* bufferPosition;
   /// True if 'bufferPosition' points to the root of a BSON document.
   bool isRoot;
   /// Index of the current item while iterating over an array.
   int index;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BSONITERATOR_H__
