#ifndef __FROG__BSONVALUE_H__
#define __FROG__BSONVALUE_H__

#include "FrogMemory.h"
#include "Table.h"
#include "Map.h"
#include "Point2.h"
#include "Point3.h"
#include "Box2.h"
#include "Box3.h"
#include "Color.h"
#include "Quaternion.h"

namespace Webfoot {
  
//==============================================================================

/// BSONValue is much like JSONValue, but it supports some of the additional
/// BSON data types.
///
/// Static blocks of BSON data can be examined with BSONIterator, which doesn't
/// involve lots of time-consuming allocations, like with BSONValue or
/// JSONValue.  BSON can also natively store binary data.  In cases where these
/// characteristics aren't relevant, you should typically use JSON instead,
/// because it's human-readable and there are more tools that can read and
/// write it.
///
/// All data types supported by JSON are supported in BSON, so JSON files that
/// are converted to BSON and back to JSON should typically retain the same
/// data.  One exception to this is that a BSON file must use an object for its
/// root.  Another issue is that BSON supports data types that are not included
/// in JSON, so BSON files are not necessarily cleanly convertible to JSON and
/// back to BSON.  The ability to encode BSON data in JSON files is intended
/// only for debugging.
///
/// Not all data types in the BSON specification are supported by Frog.  The
/// supported data types are: double, string, document (object), array, generic
/// binary data, boolean, null, int32, and int64.
class BSONValue
{
public:
   enum ValueType
   {
      /// Not defined
      TYPE_NULL,
      /// 'true' or 'false'
      TYPE_BOOLEAN,
      /// Native 'double' type.
      TYPE_DOUBLE,
      /// Native 'int32' type.
      TYPE_INT32,
      /// Native 'int64' type.
      TYPE_INT64,
      /// Null-terminated UTF-8 string.
      TYPE_STRING,
      /// Block of binary data.
      TYPE_BINARY,
      /// String-indexed collection of values.
      TYPE_OBJECT,
      /// Numerically-indexed collection of values. 
      TYPE_ARRAY
   };
   
   /// Iterator for when a BSONValue represents a string-indexed collection
   /// of values.
   typedef Map<const char*, BSONValue*>::Iterator ObjectIterator;
   
   BSONValue();
   void Init(HeapID _heapID = HEAP_DEFAULT);

   /// Clean up anything allocated by this node, and recursively Deinit and
   /// delete its descendants.
   void Deinit();
   
   /// Assuming this is an object or an array, recursively deinit and delete
   /// its descendants.
   void Clear();
   /// Recursively create and return a copy of this BSONValue and its descendants
   /// using the given heap  If the 'clone' parameter is specified, use it as
   /// the object for the clone and ignore the '_heapID' parameter.
   BSONValue* Clone(HeapID _heapID = HEAP_DEFAULT, BSONValue* clone = NULL);

   /// Assuming this is a BSON object and 'otherObject' is another BSON object,
   /// assimilate 'otherObject'.  All key/value pairs in 'otherObject' will be
   /// moved into this one, overwriting any duplicates.  'otherObject' itself
   /// will be deinited and deleted.  Since this deletes 'otherObject' but not
   /// its descendants, this can cause some fragmentation.
   void Merge(BSONValue* otherObject);
   
   /// If this is an Array or an Object, return the number of elements
   /// in the collection.  If this is a binary block, return the size in bytes.
   int SizeGet();
   /// Assuming this is a BSON array, resize to the given number of elements.
   /// Create BSONValues of TYPE_NULL or delete BSONValues as needed.
   void SizeSet(int newSize);

   /// Assuming this is a BSON array, allocate space for this many elements.
   void Reserve(int reserveSize);

   /// Return the underlying type of data for this value.
   ValueType ValueTypeGet() { return _valueType; }
   
   /// Make this a BSONValue of TYPE_NULL.
   BSONValue& NullSet();
   /// Make this BSONValue a boolean and set it to the given value.
   BSONValue& Set(bool value);
   /// Make this BSONValue an int32 and set it to the given value.
   BSONValue& Set(int32 value);
   /// Make this BSONValue an int64 and set it to the given value.
   BSONValue& Set(int64 value);
   /// Make this BSONValue a double and set it to the given value.
   BSONValue& Set(double value);
   /// Make this BSONValue a string and set it to a copy of the given null-terminated string.
   BSONValue& Set(const char* value);
   /// Make this BSONValue a string and set it to a copy of the given non-null-terminated string.
   BSONValue& Set(const char* value, size_t valueLength);
   /// Make this BSONValue a binary block and set it to a copy of the given binary data.
   BSONValue& Set(const void* value, size_t valueSize);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const Point2I& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const Point2F& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const Point3F& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const Box2F& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const Box3F& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const ColorRGBA8& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const ColorRGBA32F& value);
   /// Set this BSONValue to store the given value.
   BSONValue& Set(const Quaternion& value);
   /// Make this BSONValue a BSON object.
   BSONValue& ObjectSet();
   /// Make this BSONValue a BSON array.
   BSONValue& ArraySet();
   /// Make this BSONValue a clone of the given BSONValue.
   BSONValue& Set(BSONValue* value);
   /// Helper for updating values concisely in certain situations.
   template<typename NewValueType>
   BSONValue& operator=(NewValueType newValue) { return Set(newValue); }
   
   /// Assuming this is a BSON array, set the item at the given index to the 
   /// given value.  If the given index is greater than the current maximum
   /// index, a new BSONValue instance will be created for the specified value, 
   /// and new BSONValues of TYPE_NULL will be created if needed to fill any gap.
   /// If the new value is a BSONValue*, it is inserted directly into the
   /// array, so it must not already have a parent.
   template<typename NewValueType>
   BSONValue& Set(int index, NewValueType newValue) { BSONValue* bsonValue = SetHelper(index); if(bsonValue) return bsonValue->Set(newValue); else return dummyValue; }
   /// Specialization of 'Set' for BSONValue*.
   BSONValue& Set(int index, BSONValue* newValue);
   /// Assuming this is a BSON object, set the item at the given key to the
   /// given value.  A new BSONValue will be created if needed.
   /// If the new value is a BSONValue*, it is inserted directly into the
   /// object, so it must not already have a parent.
   template<typename NewValueType>
   BSONValue& Set(const char* key, NewValueType newValue) { BSONValue* bsonValue = SetHelper(key); if(bsonValue) return bsonValue->Set(newValue); else return dummyValue; }
   /// Specialization of 'Set' for BSONValue*.
   BSONValue& Set(const char* key, BSONValue* newValue);
   /// Form of 'Set' for non-null-terminated strings.
   BSONValue& Set(int index, const char* newValue, size_t valueLength) { BSONValue* bsonValue = SetHelper(index); if(bsonValue) return bsonValue->Set(newValue, valueLength); else return dummyValue; }
   /// Form of 'Set' for non-null-terminated strings.
   BSONValue& Set(const char* key, const char* newValue, size_t valueLength) { BSONValue* bsonValue = SetHelper(key); if(bsonValue) return bsonValue->Set(newValue, valueLength); else return dummyValue; }
   /// Form of 'Set' for binary data.
   BSONValue& Set(int index, const void* newValue, size_t valueSize) { BSONValue* bsonValue = SetHelper(index); if(bsonValue) return bsonValue->Set(newValue, valueSize); else return dummyValue; }
   /// Form of 'Set' for binary data.
   BSONValue& Set(const char* key, const void* newValue, size_t valueSize) { BSONValue* bsonValue = SetHelper(key); if(bsonValue) return bsonValue->Set(newValue, valueSize); else return dummyValue; }
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a BSONValue of TYPE_NULL.
   BSONValue& NullSet(int index) { BSONValue* bsonValue = SetHelper(index); if(bsonValue) return bsonValue->NullSet(); else return dummyValue; }
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a BSONValue of TYPE_NULL.
   BSONValue& NullSet(const char* key) { BSONValue* bsonValue = SetHelper(key); if(bsonValue) return bsonValue->NullSet(); else return dummyValue; }
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a BSON object.  Return a reference to the BSONValue that was set.
   BSONValue& ObjectSet(int index);
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a BSON object.  Return a reference to the BSONValue that was set.
   BSONValue& ObjectSet(const char* key);
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a BSON array.  Return a reference to the BSONValue that was set.
   BSONValue& ArraySet(int index);
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a BSON array.  Return a reference to the BSONValue that was set.
   BSONValue& ArraySet(const char* key);

   /// Assuming this is an array, return the value at the given index.  If you
   /// call this method with an index that is not valid, it will return a
   /// reference to a shared dummy BSONValue of TYPE_NULL.  Do not attempt to
   /// write to the dummy value.  If you need to know whether a given entry is
   /// actually defined or what type it is, use NullCheck, NumberCheck, etc.
   BSONValue& Get(int index);
   /// Assuming this is an object, return the value with the given key.  If you
   /// call this method with a key that is not valid, it will return a
   /// reference to a shared dummy BSONValue of TYPE_NULL.  Do not attempt to
   /// write to the dummy value.  If you need to know whether a given entry is
   /// actually defined or what type it is, use NullCheck, NumberCheck, etc.
   BSONValue& Get(const char* key);

   /// Assuming this is an array, return the item at the given index.  If it
   /// does not already exist, create it as a BSONValue of type null.
   BSONValue& GetOrNullSet(int index) { if(NullCheck(index)) return NullSet(index); else return Get(index); }
   /// Assuming this is an object, return the item with the given key.  If it
   /// does not already exist, create it as a BSONValue of type null.
   BSONValue& GetOrNullSet(const char* key) { if(NullCheck(key)) return NullSet(key); else return Get(key); }
   
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
   operator const char*() { return StringCheck() ? stringValue : NULL; }
   /// Convert the object to a block of binary data.  Defaults to NULL.
   operator const void*() { return BinaryCheck() ? binaryValue : NULL; }
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
   /// Convert the object to a pointer to the object.  This allows the Get
   /// methods to return a BSONValue*, in a way.
   operator BSONValue*() { return this; }

   /// Assuming this is an array, Deinit and delete the item at the given index.
   /// Values of higher indices will move down to fill the gap.
   void Remove(int index);
   /// Assuming this is an object, Deinit and delete the item with the given key.
   void Remove(const char* key);
   /// Assuming this is an object, Deinit and delete the item at the given
   /// iterator position, and update the iterator to the following element.
   void Remove(ObjectIterator& iterator);

   /// Assuming this is a BSON array, add the given value to the back of the
   /// collection.  If the new value is a BSONValue*, it is inserted directly
   /// into the array, so it must not already have a parent.
   template<typename NewValueType>
   BSONValue& Add(NewValueType newValue) { return Set(SizeGet(), newValue); }
   /// Specialization of 'Add' for BSONValue*.
   BSONValue& Add(BSONValue* newValue);
   /// Form of Add for non-null-terminated strings.
   BSONValue& Add(const char* newValue, size_t valueLength) { return Set(SizeGet(), newValue, valueLength); }
   /// Form of Add for binary data.
   BSONValue& Add(const void* newValue, size_t valueSize) { return Set(SizeGet(), newValue, valueSize); }
   /// Assuming this is a BSON array, add a new BSONValue of TYPE_NULL to the
   /// back of the collection.
   BSONValue& NullAdd() { return NullSet(SizeGet()); }
   /// Assuming this is a BSON array, add a new BSON array to the back of the
   /// collection.   Return a reference to the BSONValue that was added.
   BSONValue& ArrayAdd() { return ArraySet(SizeGet()); }
   /// Assuming this is a BSON array, add a new BSON object to the back of the
   /// collection.  Return a reference to the BSONValue that was added.
   BSONValue& ObjectAdd() { return ObjectSet(SizeGet()); }

   /// Assuming this is a BSON array, set the item at the given index to the
   /// given value.  If the given index is greater than the current maximum
   /// index, a new BSONValue instance will be created for the specified value,
   /// and new BSONValues of TYPE_NULL will be created if needed to fill any gap.
   /// If the new value is a BSONValue*, it is inserted directly into the
   /// array, so it must not already have a parent.
   template<typename NewValueType>
   BSONValue& Insert(int index, NewValueType newValue)
   {
      if(index > SizeGet())
         SizeSet(index);
      BSONValue& returnValue = Add(newValue);
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// Form of 'Set' for non-null-terminated strings.
   BSONValue& Insert(int index, const char* newValue, size_t valueLength)
   {
      if(index > SizeGet())
         SizeSet(index);
      BSONValue& returnValue = Add(newValue, valueLength);
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// Form of 'Set' for binary data.
   BSONValue& Insert(int index, const void* newValue, size_t valueSize)
   {
      if(index > SizeGet())
         SizeSet(index);
      BSONValue& returnValue = Add(newValue, valueSize);
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// This follows the same rules as the other Insert methods, except it makes
   /// the element a BSONValue of TYPE_NULL.
   BSONValue& NullInsert(int index)
   {
      if(index > SizeGet())
         SizeSet(index);
      BSONValue& returnValue = NullAdd();
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// This follows the same rules as the other Insert methods, except it makes
   /// the element a BSON object.  Return a reference to the BSONValue that was set.
   BSONValue& ObjectInsert(int index)
   {
      if(index > SizeGet())
         SizeSet(index);
      BSONValue& returnValue = ObjectAdd();
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// This follows the same rules as the other Insert methods, except it makes
   /// the element a BSON array.  Return a reference to the BSONValue that was set.
   BSONValue& ArrayInsert(int index)
   {
      if(index > SizeGet())
         SizeSet(index);
      BSONValue& returnValue = ArrayAdd();
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   
   /// Return true if this is not a defined value.
   bool NullCheck() { return _valueType == TYPE_NULL; }
   /// Assuming this is a BSON array, return true if the value at the given
   /// index is not defined.
   bool NullCheck(int index)
   {
      BSONValue* value = ElementGet(index);
      return !value || value->NullCheck();
   }
   /// Assuming this is a BSON object, return true if the value at the given key
   /// is not defined.
   bool NullCheck(const char* key)
   {
      BSONValue* value = ElementGet(key);
      return !value || value->NullCheck();
   }
   
   /// Return true if this represents a boolean.
   bool BooleanCheck() { return _valueType == TYPE_BOOLEAN; }
   /// Assuming this is a BSON array, return true if the value at the given index represents a boolean.
   bool BooleanCheck(int index) { BSONValue* value = ElementGet(index); return value && value->BooleanCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a boolean.
   bool BooleanCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->BooleanCheck(); }

   /// Return true if this represents a double.
   bool DoubleCheck() { return _valueType == TYPE_DOUBLE; }
   /// Assuming this is a BSON array, return true if the value at the given index represents a double.
   bool DoubleCheck(int index) { BSONValue* value = ElementGet(index); return value && value->DoubleCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a double.
   bool DoubleCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->DoubleCheck(); }

   /// Return true if this represents an int32.
   bool Int32Check() { return _valueType == TYPE_INT32; }
   /// Assuming this is a BSON array, return true if the value at the given index represents an int32.
   bool Int32Check(int index) { BSONValue* value = ElementGet(index); return value && value->Int32Check(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents an int32.
   bool Int32Check(const char* key) { BSONValue* value = ElementGet(key); return value && value->Int32Check(); }

   /// Return true if this represents an int64.
   bool Int64Check() { return _valueType == TYPE_INT64; }
   /// Assuming this is a BSON array, return true if the value at the given index represents an int64.
   bool Int64Check(int index) { BSONValue* value = ElementGet(index); return value && value->Int64Check(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents an int64.
   bool Int64Check(const char* key) { BSONValue* value = ElementGet(key); return value && value->Int64Check(); }
   
   /// Return true if this represents a number.
   bool NumberCheck() { return (_valueType == TYPE_DOUBLE) || (_valueType == TYPE_INT32) || (_valueType == TYPE_INT64); }
   /// Assuming this is a BSON array, return true if the value at the given index represents a number.
   bool NumberCheck(int index) { BSONValue* value = ElementGet(index); return value && value->NumberCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a number.
   bool NumberCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->NumberCheck(); }
   
   /// Return true if this represents a string.
   bool StringCheck() { return _valueType == TYPE_STRING; }
   /// Assuming this is a BSON array, return true if the value at the given index represents a string.
   bool StringCheck(int index) { BSONValue* value = ElementGet(index); return value && value->StringCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a string.
   bool StringCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->StringCheck(); }

   /// Return true if this represents a block of binary data.
   bool BinaryCheck() { return _valueType == TYPE_BINARY; }
   /// Assuming this is a BSON array, return true if the value at the given index represents a block of binary data..
   bool BinaryCheck(int index) { BSONValue* value = ElementGet(index); return value && value->BinaryCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a block of binary data..
   bool BinaryCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->BinaryCheck(); }
   
   /// Return true if this can be interpreted as a Point2I.
   bool Point2ICheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Point2I.
   bool Point2ICheck(int index) { BSONValue* value = ElementGet(index); return value && value->Point2ICheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Point2F.
   bool Point2ICheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->Point2ICheck(); }
   
   /// Return true if this can be interpreted as a Point2F.
   bool Point2FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Point2F.
   bool Point2FCheck(int index) { BSONValue* value = ElementGet(index); return value && value->Point2FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Point2F.
   bool Point2FCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->Point2FCheck(); }

   /// Return true if this can be interpreted as a Point3F.
   bool Point3FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Point3F.
   bool Point3FCheck(int index) { BSONValue* value = ElementGet(index); return value && value->Point3FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Point3F.
   bool Point3FCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->Point3FCheck(); }
   
   /// Return true if this can be interpreted as a Box2F.
   bool Box2FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Box2F.
   bool Box2FCheck(int index) { BSONValue* value = ElementGet(index); return value && value->Box2FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Box2F.
   bool Box2FCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->Box2FCheck(); }
   
   /// Return true if this can be interpreted as a Box3F.
   bool Box3FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Box3F.
   bool Box3FCheck(int index) { BSONValue* value = ElementGet(index); return value && value->Box3FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Box3F.
   bool Box3FCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->Box3FCheck(); }
   
   /// Return true if this can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check(int index) { BSONValue* value = ElementGet(index); return value && value->ColorRGBA8Check(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check(const char* key) { BSONValue* value = ElementGet(key); return value && value->ColorRGBA8Check(); }

   /// Return true if this can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck(int index) { BSONValue* value = ElementGet(index); return value && value->ColorRGBA32FCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->ColorRGBA32FCheck(); }

   /// Return true if this can be interpreted as a Quaternion.
   bool QuaternionCheck();
   /// Assuming this is a BSON array, return true if the value at the given index can be interpreted as a Quaternion.
   bool QuaternionCheck(int index) { BSONValue* value = ElementGet(index); return value && value->QuaternionCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key can be interpreted as a Quaternion.
   bool QuaternionCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->QuaternionCheck(); }

   /// Return true if this represents a BSON array.
   bool ArrayCheck() { return _valueType == TYPE_ARRAY; }
   /// Assuming this is a BSON array, return true if the value at the given index represents a BSON array.
   bool ArrayCheck(int index) { BSONValue* value = ElementGet(index); return value && value->ArrayCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a BSON array.
   bool ArrayCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->ArrayCheck(); }
   
   /// Return true if this represents a BSON object.
   bool ObjectCheck() { return _valueType == TYPE_OBJECT; }
   /// Assuming this is a BSON array, return true if the value at the given index represents a BSON object.
   bool ObjectCheck(int index) { BSONValue* value = ElementGet(index); return value && value->ObjectCheck(); }
   /// Assuming this is a BSON object, return true if the value at the given key represents a BSON object.
   bool ObjectCheck(const char* key) { BSONValue* value = ElementGet(key); return value && value->ObjectCheck(); }

   /// Assuming this is a BSON array, move the item at the given 'oldIndex' to
   /// the given 'newIndex'.  Items will be shifted as needed.
   void Move(int newIndex, int oldIndex);
   /// Assuming this is a BSON object, move the item at the given 'oldKey' to
   /// the given 'newKey'.  If there is already an item at 'newKey', it will be
   /// overwritten.
   void Move(const char* newKey, const char* oldKey);

   /// Assuming this is an array, return the index of the first instance of the
   /// given value in the array.  Return -1 if it is not found.
   int FindIndex(int value);
   /// Assuming this is an array, return the index of the first instance of the
   /// given value in the array.  Return -1 if it is not found.
   int FindIndex(const char* value);
   
   /// Assuming this is a BSON object, return an iterator at the beginning of
   /// the collection.  If this is not a BSON object, the behavior is undefined.
   ObjectIterator ObjectBegin();

   /// Return the heap used for all allocations by this instance.
   HeapID HeapIDGet() { return heapID; }
   
protected:
   /// Assuming this is a BSON array, return a pointer to the item at the given
   /// index, or NULL if the index is invalid.
   BSONValue* ElementGet(int index);
   /// Assuming this is a BSON object, return a pointer to the item at the given
   /// key, or NULL if there is nothing at the given key (not even a BSONValue
   /// of TYPE_NULL).
   BSONValue* ElementGet(const char* key);
   
   /// Assuming this is a BSON array, return a pointer to the value at the given
   /// index if there is already a value at that index.  If the given index is
   /// greater than the current maximum index, new BSONValues of TYPE_NULL will
   /// be created as needed, and the value at the requested index will be returned.
   BSONValue* SetHelper(int index);
   /// Assuming this is a BSON object, return a pointer to the value with the
   /// given key if there is already a value for that key.  If not, create and
   /// return a new BSONValue of TYPE_NULL at the given key.  
   BSONValue* SetHelper(const char* key);
   
   /// The basic type of BSONValue this instance represents.
   /// We include the underscore to compensate for what seems to be a limitation of the
   /// custom visualization system in the Visual Studio 2005 debugger (autoexp.dat).
   ValueType _valueType;
   /// The underlying value for this instance.
   union
   {
      bool booleanValue;
      double doubleValue;
      int32 int32Value;
      int64 int64Value;
      const char* stringValue;
      void* binaryValue;
      Map<const char*, BSONValue*>* objectValue;
      Table<BSONValue*>* arrayValue;
   };
   /// Size of the buffer pointed to by 'binaryValue'.
   size_t binaryValueSize;
   /// Heap used for all allocations by this instance.
   HeapID heapID;
   /// Returned by the Get methods if an entry isn't defined.
   static BSONValue dummyValue;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__BSONVALUE_H__
