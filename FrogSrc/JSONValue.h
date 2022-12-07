#ifndef __FROG__JSONVALUE_H__
#define __FROG__JSONVALUE_H__

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

/// Extension to use on JSON files.
#define JSON_FILE_EXTENSION "json"
/// Default string for specifying a parent in JSONValue::BuildRecursive.
#define JSON_PARENT_KEY_DEFAULT "Inherited"

class JSONParser;

//==============================================================================

/// JSONValue represents a node in a tree of JSON values.  The basic types of
/// values are null, boolean, number, string, object, and array.  "numbers" are
/// native doubles, "strings" are null-terminated UTF-8 strings, 
/// "objects" are maps of strings to JSONValues, and "arrays" are
/// numerically-indexed tables of JSONValues.  This implementation includes
/// methods for additional types, but these are all ultimately represented
/// using the basic JSON value types.
///
/// To check whether a value is defined or if it is of a certain type, use
/// NullCheck, BoolCheck, etc.  If you're confident that a given value is
/// defined and that it is of the correct type, you can request it.  For
/// example...
///
///    const char* title = NULL;
///    if(root->StringCheck("Title"))
///       title = root->Get("Title");
///    else
///       title = TITLE_DEFAULT;
///
/// As you can see, while the value returned by Get is a JSONValue&, it is
/// implicitly converted to a const char*.  A JSONValue reference (not a
/// pointer) can be cast to various types, including a JSONValue pointer.  This
/// allows you to use a single method to query for a boolean, integer, float,
/// double, string, JSONValue*, etc.  If you try to convert a JSONValue to a type
/// other than the one it is actually representing, it will fall back on a
/// default.  For example, if you had a JSONValue that was a boolean and you
/// cast it to a const char*, the resulting value would be NULL.
///
/// To access members of a tree of JSONValues that are nested, use the Get
/// methods.  For example, to get the Color for the Moose in the following...
///
///    {
///       "Moose":
///       {
///          "Color": "255|0|0|255"
///       },
///       "PolarBear":
///       {
///          "Color": "0|0|255|255"
///       }
///    }
///
/// ...you could use...
///
///    JSONValue* mooseDetails = root->Get("Moose");
///    ColorRGBA8 mooseColor = mooseDetails->Get("Color");
///
/// ...or...
///
///    ColorRGBA8 mooseColor = root->Get("Moose").Get("Color");
///
/// To create a JSONValue from scratch, just instantiate the class and call
/// Init.  By default, JSONValues make their allocations from the default
/// heap, but this can be overridden in the call to Init.  Newly created
/// JSONValues are of TYPE_NULL.  To give it a value, use one of the Set
/// methods (Set, NullSet, ArraySet, ObjectSet).  If you want to build a tree
/// if values, use the Set and Add methods.  For example, to build the
/// above structure with the polar bear and the moose, you could do the
/// following...
///
///    // Set up the root.
///    JSONValue* root = frog_new JSONValue();
///    root->Init();
///    root->ObjectSet();
///
///    // Add the Moose object.
///    JSONValue* mooseDetails = root->ObjectSet("Moose");
///    ColorRGBA8 mooseColor = {255, 0, 0, 255};
///    mooseDetails->Set("Color", mooseColor);
///
///    // Add the PolarBear object.
///    JSONValue* polarBearDetails = root->ObjectSet("PolarBear");
///    ColorRGBA8 polarBearColor = {0, 0, 255, 255};
///    polarBearDetails->Set("Color", polarBearColor);
class JSONValue
{
public:
   enum ValueType
   {
      /// Not defined
      TYPE_NULL,
      /// 'true' or 'false'
      TYPE_BOOLEAN,
      /// Native 'double' type.
      TYPE_NUMBER,
      /// Null-terminated UTF-8 string.
      TYPE_STRING,
      /// String-indexed collection of values.
      TYPE_OBJECT,
      /// Numerically-indexed collection of values. 
      TYPE_ARRAY
   };
   
   /// Iterator for when a JSONValue represents a string-indexed collection
   /// of values.
   typedef Map<const char*, JSONValue*>::Iterator ObjectIterator;
   
   JSONValue();
   void Init(HeapID _heapID = HEAP_DEFAULT);

   /// Clean up anything allocated by this node, and recursively Deinit and
   /// delete its descendants.
   void Deinit();
   
   /// Assuming this is an object or an array, recursively deinit and delete
   /// its descendants.
   void Clear();
   /// Recursively create and return a copy of this JSONValue and its descendants
   /// using the given heap  If the 'clone' parameter is specified, use it as
   /// the object for the clone and ignore the '_heapID' parameter.
   JSONValue* Clone(HeapID _heapID = HEAP_DEFAULT, JSONValue* clone = NULL);

   /// If this is a JSON object and 'otherValue' is another JSON object,
   /// assimilate 'otherValue'.  All key/value pairs in 'otherValue' will be
   /// moved into this one, overwriting any duplicates.
   ///
   /// If this is a JSON array and 'otherValue' is another JSON array, move all
   /// items from 'otherValue' to the end of this array.
   ///
   /// In either case, 'otherValue' itself will be deinited and deleted.  Since
   /// this deletes 'otherValue' but not its descendants, this can cause some
   /// fragmentation.
   void Merge(JSONValue* otherValue);
   
   /// Assuming this is an Array or an Object, return the number of elements
   /// in the collection.
   int SizeGet() const;
   /// Assuming this is a JSON array, resize to the given number of elements.
   /// Create JSONValues of TYPE_NULL or delete JSONValues as needed.
   void SizeSet(int newSize);

   /// Assuming this is a JSON array, allocate space for this many elements.
   void Reserve(int reserveSize);

   /// Return the underlying type of data for this value.
   ValueType ValueTypeGet() const { return _valueType; }
   
   /// Make this a JSONValue of TYPE_NULL.
   JSONValue& NullSet();
   /// Make this JSONValue a boolean and set it to the given value.
   JSONValue& Set(bool value);
   /// Make this JSONValue a number and set it to the given value.
   JSONValue& Set(int value);
   /// Make this JSONValue a number and set it to the given value.
   JSONValue& Set(unsigned int value);
   /// Make this JSONValue a number and set it to the given value.
   JSONValue& Set(float value);
   /// Make this JSONValue a number and set it to the given value.
   JSONValue& Set(double value);
   /// Make this JSONValue a string and set it to a copy of the given null-terminated string.
   JSONValue& Set(const char* value);
   /// Make this JSONValue a string and set it to a copy of the given non-null-terminated string.
   JSONValue& Set(const char* value, size_t valueLength);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const Point2I& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const Point2F& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const Point3F& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const Box2F& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const Box3F& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const ColorRGBA8& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const ColorRGBA32F& value);
   /// Set this JSONValue to store the given value.
   JSONValue& Set(const Quaternion& value);
   /// Make this JSONValue a JSON object.
   JSONValue& ObjectSet();
   /// Make this JSONValue a JSON array.
   JSONValue& ArraySet();
   /// Make this JSONValue a clone of the given JSONValue.
   JSONValue& Set(JSONValue* value);
   /// Helper for updating values concisely in certain situations.
   template<typename NewValueType>
   JSONValue& operator=(NewValueType newValue) { return Set(newValue); }
   
   /// Assuming this is a JSON array, set the item at the given index to the 
   /// given value.  If the given index is greater than the current maximum
   /// index, a new JSONValue instance will be created for the specified value, 
   /// and new JSONValues of TYPE_NULL will be created if needed to fill any gap.
   /// If the new value is a JSONValue*, it is inserted directly into the
   /// array, so it must not already have a parent.
   template<typename NewValueType>
   JSONValue& Set(int index, NewValueType newValue) { JSONValue* jsonValue = SetHelper(index); if(jsonValue) return jsonValue->Set(newValue); else return dummyValue; }
   /// Specialization of 'Set' for JSONValue*.
   JSONValue& Set(int index, JSONValue* newValue);
   /// Assuming this is a JSON object, set the item at the given key to the
   /// given value.  A new JSONValue will be created if needed.
   /// If the new value is a JSONValue*, it is inserted directly into the
   /// object, so it must not already have a parent.
   template<typename NewValueType>
   JSONValue& Set(const char* key, NewValueType newValue) { JSONValue* jsonValue = SetHelper(key); if(jsonValue) return jsonValue->Set(newValue); else return dummyValue; }
   /// Specialization of 'Set' for JSONValue*.
   JSONValue& Set(const char* key, JSONValue* newValue);
   /// Form of 'Set' for non-null-terminated strings.
   JSONValue& Set(int index, const char* newValue, size_t valueLength) { JSONValue* jsonValue = SetHelper(index); if(jsonValue) return jsonValue->Set(newValue, valueLength); else return dummyValue; }
   /// Form of 'Set' for non-null-terminated strings.
   JSONValue& Set(const char* key, const char* newValue, size_t valueLength) { JSONValue* jsonValue = SetHelper(key); if(jsonValue) return jsonValue->Set(newValue, valueLength); else return dummyValue; }
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a JSONValue of TYPE_NULL.
   JSONValue& NullSet(int index) { JSONValue* jsonValue = SetHelper(index); if(jsonValue) return jsonValue->NullSet(); else return dummyValue; }
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a JSONValue of TYPE_NULL.
   JSONValue& NullSet(const char* key) { JSONValue* jsonValue = SetHelper(key); if(jsonValue) return jsonValue->NullSet(); else return dummyValue; }
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a JSON object.  Return a reference to the JSONValue that was set.
   JSONValue& ObjectSet(int index);
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a JSON object.  Return a reference to the JSONValue that was set.
   JSONValue& ObjectSet(const char* key);
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a JSON array.  Return a reference to the JSONValue that was set.
   JSONValue& ArraySet(int index);
   /// This follows the same rules as the other Set methods, except it makes
   /// the element a JSON array.  Return a reference to the JSONValue that was set.
   JSONValue& ArraySet(const char* key);

   /// Assuming this is an array, return the value at the given index.  If you
   /// call this method with an index that is not valid, it will return a
   /// reference to a shared dummy JSONValue of TYPE_NULL.  Do not attempt to
   /// write to the dummy value.  If you need to know whether a given entry is
   /// actually defined or what type it is, use NullCheck, NumberCheck, etc.
   JSONValue& Get(int index);
   /// Assuming this is an object, return the value with the given key.  If you
   /// call this method with a key that is not valid, it will return a
   /// reference to a shared dummy JSONValue of TYPE_NULL.  Do not attempt to
   /// write to the dummy value.  If you need to know whether a given entry is
   /// actually defined or what type it is, use NullCheck, NumberCheck, etc.
   JSONValue& Get(const char* key);

   /// Assuming this is an array, return the item at the given index.  If it
   /// does not already exist, create it as a JSONValue of type null.
   JSONValue& GetOrNullSet(int index) { if(NullCheck(index)) return NullSet(index); else return Get(index); }
   /// Assuming this is an object, return the item with the given key.  If it
   /// does not already exist, create it as a JSONValue of type null.
   JSONValue& GetOrNullSet(const char* key) { if(NullCheck(key)) return NullSet(key); else return Get(key); }

   /// Assuming this is an array, return the first child which is an object with
   /// the given 'childValue' for the given 'key'.  If no matching child is
   /// found, it will return a reference to a shared dummy JSONValue of
   /// TYPE_NULL.  Do not attempt to write to the dummy value.
   JSONValue& GetByChildKeyAndValue(const char* key, const char* value);
   
   /// Convert the object to a bool.  If the underlying value is actually a
   /// non-zero number, it will return true.  Defaults to false.
   operator bool();
   /// Convert the object to an int.  If the underlying value is actually a
   /// boolean, it will return 1 for true and 0 for false.  Defaults to 0.
   operator int();
   /// Convert the object to an unsigned int.  If the underlying value is actually a
   /// boolean, it will return 1 for true and 0 for false.  Defaults to 0.
   operator unsigned int();
   /// Convert the object to a float.  If the underlying value is actually a
   /// boolean, it will return 1.0f for true and 0.0f for false.  Defaults to 0.0f.
   operator float();
   /// Convert the object to a double.  If the underlying value is actually a
   /// boolean, it will return 1.0 for true and 0.0 for false.  Defaults to 0.0.
   operator double();
   /// Convert the object to a string.  Defaults to NULL.
   operator const char*() { return StringCheck() ? stringValue : NULL; }
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
   /// methods to return a JSONValue*, in a way.
   operator JSONValue*() { return this; }

   /// Return true if the two are equivalient
   bool operator==(JSONValue& other);
   /// Return true if the two are not equivalient
   bool operator!=(JSONValue& other) { return !(*this == other); }

   /// Assuming this is an array, Deinit and delete the item at the given index.
   /// Values of higher indices will move down to fill the gap.
   void Remove(int index);
   /// Assuming this is an object, Deinit and delete the item with the given key.
   void Remove(const char* key);
   /// Assuming this is an object, Deinit and delete the item at the given
   /// iterator position, and update the iterator to the following element.
   void Remove(ObjectIterator& iterator);

   /// Assuming this is an array, remove the item at the given index without
   /// deiniting or deleting it.  Values of higher indices will move down to
   /// fill the gap.
   void Detach(int index);
   /// Assuming this is an object, remove the item at the given key without
   /// deiniting or deleting it.
   void Detach(const char* key);
   
   /// Assuming this is a JSON array, add the given value to the back of the
   /// collection.  If the new value is a JSONValue*, it is inserted directly
   /// into the array, so it must not already have a parent.
   template<typename NewValueType>
   JSONValue& Add(NewValueType newValue) { return Set(SizeGet(), newValue); }
   /// Specialization of 'Add' for JSONValue*.
   JSONValue& Add(JSONValue* newValue);
   /// Form of Add for non-null-terminated strings.
   JSONValue& Add(const char* newValue, size_t valueLength) { return Set(SizeGet(), newValue, valueLength); }
   /// Assuming this is a JSON array, add a new JSONValue of TYPE_NULL to the
   /// back of the collection.
   JSONValue& NullAdd() { return NullSet(SizeGet()); }
   /// Assuming this is a JSON array, add a new JSON array to the back of the
   /// collection.   Return a reference to the JSONValue that was added.
   JSONValue& ArrayAdd() { return ArraySet(SizeGet()); }
   /// Assuming this is a JSON array, add a new JSON object to the back of the
   /// collection.  Return a reference to the JSONValue that was added.
   JSONValue& ObjectAdd() { return ObjectSet(SizeGet()); }

   /// Assuming this is a JSON array, set the item at the given index to the
   /// given value.  If the given index is greater than the current maximum
   /// index, a new JSONValue instance will be created for the specified value,
   /// and new JSONValues of TYPE_NULL will be created if needed to fill any gap.
   /// If the new value is a JSONValue*, it is inserted directly into the
   /// array, so it must not already have a parent.
   template<typename NewValueType>
   JSONValue& Insert(int index, NewValueType newValue)
   {
      if(index > SizeGet())
         SizeSet(index);
      JSONValue& returnValue = Add(newValue);
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// Form of 'Set' for non-null-terminated strings.
   JSONValue& Insert(int index, const char* newValue, size_t valueLength)
   {
      if(index > SizeGet())
         SizeSet(index);
      JSONValue& returnValue = Add(newValue, valueLength);
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// This follows the same rules as the other Insert methods, except it makes
   /// the element a JSONValue of TYPE_NULL.
   JSONValue& NullInsert(int index)
   {
      if(index > SizeGet())
         SizeSet(index);
      JSONValue& returnValue = NullAdd();
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// This follows the same rules as the other Insert methods, except it makes
   /// the element a JSON object.  Return a reference to the JSONValue that was set.
   JSONValue& ObjectInsert(int index)
   {
      if(index > SizeGet())
         SizeSet(index);
      JSONValue& returnValue = ObjectAdd();
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   /// This follows the same rules as the other Insert methods, except it makes
   /// the element a JSON array.  Return a reference to the JSONValue that was set.
   JSONValue& ArrayInsert(int index)
   {
      if(index > SizeGet())
         SizeSet(index);
      JSONValue& returnValue = ArrayAdd();
      Move(index, SizeGet() - 1);
      return returnValue;
   }
   
   /// Return true if this is not a defined value.
   bool NullCheck() { return _valueType == TYPE_NULL; }
   /// Assuming this is a JSON array, return true if the value at the given
   /// index is not defined.
   bool NullCheck(int index)
   {
      JSONValue* value = ElementGet(index);
      return !value || value->NullCheck();
   }
   /// Assuming this is a JSON object, return true if the value at the given key
   /// is not defined.
   bool NullCheck(const char* key)
   {
      JSONValue* value = ElementGet(key);
      return !value || value->NullCheck();
   }
   
   /// Return true if this represents a boolean.
   bool BooleanCheck() { return _valueType == TYPE_BOOLEAN; }
   /// Assuming this is a JSON array, return true if the value at the given index represents a boolean.
   bool BooleanCheck(int index) { JSONValue* value = ElementGet(index); return value && value->BooleanCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key represents a boolean.
   bool BooleanCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->BooleanCheck(); }
   
   /// Return true if this represents a number.
   bool NumberCheck() { return _valueType == TYPE_NUMBER; }
   /// Assuming this is a JSON array, return true if the value at the given index represents a number.
   bool NumberCheck(int index) { JSONValue* value = ElementGet(index); return value && value->NumberCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key represents a number.
   bool NumberCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->NumberCheck(); }
   
   /// Return true if this represents a string.
   bool StringCheck() { return _valueType == TYPE_STRING; }
   /// Assuming this is a JSON array, return true if the value at the given index represents a string.
   bool StringCheck(int index) { JSONValue* value = ElementGet(index); return value && value->StringCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key represents a string.
   bool StringCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->StringCheck(); }
   
   /// Return true if this can be interpreted as a Point2I.
   bool Point2ICheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a Point2I.
   bool Point2ICheck(int index) { JSONValue* value = ElementGet(index); return value && value->Point2ICheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a Point2F.
   bool Point2ICheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->Point2ICheck(); }
   
   /// Return true if this can be interpreted as a Point2F.
   bool Point2FCheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a Point2F.
   bool Point2FCheck(int index) { JSONValue* value = ElementGet(index); return value && value->Point2FCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a Point2F.
   bool Point2FCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->Point2FCheck(); }

   /// Return true if this can be interpreted as a Point3F.
   bool Point3FCheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a Point3F.
   bool Point3FCheck(int index) { JSONValue* value = ElementGet(index); return value && value->Point3FCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a Point3F.
   bool Point3FCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->Point3FCheck(); }
   
   /// Return true if this can be interpreted as a Box2F.
   bool Box2FCheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a Box2F.
   bool Box2FCheck(int index) { JSONValue* value = ElementGet(index); return value && value->Box2FCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a Box2F.
   bool Box2FCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->Box2FCheck(); }
   
   /// Return true if this can be interpreted as a Box3F.
   bool Box3FCheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a Box3F.
   bool Box3FCheck(int index) { JSONValue* value = ElementGet(index); return value && value->Box3FCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a Box3F.
   bool Box3FCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->Box3FCheck(); }
   
   /// Return true if this can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check(int index) { JSONValue* value = ElementGet(index); return value && value->ColorRGBA8Check(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a ColorRGBA8.
   bool ColorRGBA8Check(const char* key) { JSONValue* value = ElementGet(key); return value && value->ColorRGBA8Check(); }

   /// Return true if this can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck(int index) { JSONValue* value = ElementGet(index); return value && value->ColorRGBA32FCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a ColorRGBA32F.
   bool ColorRGBA32FCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->ColorRGBA32FCheck(); }

   /// Return true if this can be interpreted as a Quaternion.
   bool QuaternionCheck();
   /// Assuming this is a JSON array, return true if the value at the given index can be interpreted as a Quaternion.
   bool QuaternionCheck(int index) { JSONValue* value = ElementGet(index); return value && value->QuaternionCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key can be interpreted as a Quaternion.
   bool QuaternionCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->QuaternionCheck(); }

   /// Return true if this represents a JSON array.
   bool ArrayCheck() { return _valueType == TYPE_ARRAY; }
   /// Assuming this is a JSON array, return true if the value at the given index represents a JSON array.
   bool ArrayCheck(int index) { JSONValue* value = ElementGet(index); return value && value->ArrayCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key represents a JSON array.
   bool ArrayCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->ArrayCheck(); }
   
   /// Return true if this represents a JSON object.
   bool ObjectCheck() { return _valueType == TYPE_OBJECT; }
   /// Assuming this is a JSON array, return true if the value at the given index represents a JSON object.
   bool ObjectCheck(int index) { JSONValue* value = ElementGet(index); return value && value->ObjectCheck(); }
   /// Assuming this is a JSON object, return true if the value at the given key represents a JSON object.
   bool ObjectCheck(const char* key) { JSONValue* value = ElementGet(key); return value && value->ObjectCheck(); }

   /// Assuming this is a JSON array, move the item at the given 'oldIndex' to
   /// the given 'newIndex'.  Items will be shifted as needed.
   void Move(int newIndex, int oldIndex);
   /// Assuming this is a JSON object, move the item at the given 'oldKey' to
   /// the given 'newKey'.  If there is already an item at 'newKey', it will be
   /// overwritten.
   void Move(const char* newKey, const char* oldKey);

   /// Assuming this is an array, return the index of the first instance of the
   /// given value in the array.  Return -1 if it is not found.
   int FindIndex(int value);
   /// Assuming this is an array, return the index of the first instance of the
   /// given value in the array.  Return -1 if it is not found.
   int FindIndex(const char* value);
   /// Assuming this is an array, return the index of the first instance of the
   /// given value in the array.  Return -1 if it is not found.
   int FindIndex(JSONValue* value);

   /// Assuming this is an array, return the index of the first child which is
   /// an object with the given 'value' for the given 'key'.  Return -1 if it
   /// is not found.
   int FindIndexByChildKeyAndValue(const char* key, const char* value);

   /// Assuming this is an array, sort the members using the given comparator.
   template<typename ComparatorType>
   void Sort(ComparatorType compare)
   {
      if((this != &dummyValue) && (_valueType == TYPE_ARRAY) && arrayValue)
         arrayValue->Sort(compare);
   }

   /// Assuming this is an array, sort the members using the given comparator
   /// leaving equivalent items in the same order.
   template<typename ComparatorType>
   void SortStable(ComparatorType compare, Allocator* tempAllocator = theAllocatorTemp)
   {
      if((this != &dummyValue) && (_valueType == TYPE_ARRAY) && arrayValue)
         arrayValue->SortStable(compare, tempAllocator);
   }
   
   /// Assuming this is a JSON object, return an iterator at the beginning of
   /// the collection.  If this is not a JSON object, the behavior is undefined.
   ObjectIterator ObjectBegin();
   /// Assuming this is a JSON object, return an iterator at the end of
   /// the collection.  If this is not a JSON object, the behavior is undefined.
   ObjectIterator ObjectBeginBack();

   /// Assuming this is an object, load a Point2F from the string with the specified key.
   /// If the point is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If 'defaultValue' is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "x|y" where both values are floats.
   bool Point2FGet(Point2F* destination, const char* key, const Point2F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other Point2FGet, except this gets the point from an index of an array.
   bool Point2FGet(Point2F* destination, int index, const Point2F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load a Point3F from the string with the specified key.
   /// If the point is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If 'defaultValue' is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "x|y|z" where all values are floats.
   bool Point3FGet(Point3F* destination, const char* key, const Point3F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other Point3FGet, except this gets the point from an index of an array.
   bool Point3FGet(Point3F* destination, int index, const Point3F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load a Quaternion from the string with the specified key.
   /// If the quaternion is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If 'defaultValue' is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "w|x|y|z" where all values are floats.
   bool QuaternionGet(Quaternion* destination, const char* key, const Quaternion* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other QuaternionGet, except this gets the point from an index of an array.
   bool QuaternionGet(Quaternion* destination, int index, const Quaternion* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load a Box2F from the string at the given key.
   /// If the string is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If default is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "x|y|width|height" where all values are floats.
   bool Box2FGet(Box2F* destination, const char* key, const Box2F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other Box2FGet, except this gets the box from an index of an array.
   bool Box2FGet(Box2F* destination, int index, const Box2F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load a Box3F from the string at the given key.
   /// If the string is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If default is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format of the string must be "x|y|width|height" where all values are floats.
   bool Box3FGet(Box3F* destination, const char* key, const Box3F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other Box3FGet, except this gets the box from an index of an array.
   bool Box3FGet(Box3F* destination, int index, const Box3F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load a ColorRGBA8 from the string at the given key.
   /// If the string is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If default is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format is "red|green|blue|alpha" where each is an integer
   /// between 0 and 255 (inclusive).
   bool ColorRGBA8Get(ColorRGBA8* destination, const char* key, const ColorRGBA8* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other ColorRGBA8Get, except this gets the color from an index of an array
   bool ColorRGBA8Get(ColorRGBA8* destination, int index, const ColorRGBA8* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load a ColorRGBA32F from the string at the given key.
   /// If the string is not correctly defined, set 'destination' to the 'defaultValue' and return false.
   /// Return true if loading is successful.  'requesterType' indicates what
   /// type of object was asking for the value.  'requesterName' indicates the name
   /// of the instance of the object that was asking for the value.  If default is NULL,
   /// the function will not attempt to use it.
   /// When 'required' is true, errors are used instead of warnings.
   /// The format is "red|green|blue|alpha" where each is floating point value
   /// normalized so that 0.0 is none and 1.0 is full.
   bool ColorRGBA32FGet(ColorRGBA32F* destination, const char* key, const ColorRGBA32F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);
   /// Same as the other ColorRGBA32FGet, except this gets the color from an index of an array
   bool ColorRGBA32FGet(ColorRGBA32F* destination, int index, const ColorRGBA32F* defaultValue = NULL,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load the float at the given key.
   /// If the item is not a number, set 'destination' to the 'defaultValue' and return false.
   /// Return true if successful.  'requesterType' indicates what type of object was asking for the value.
   /// 'requesterName' indicates the name of the instance of the object that was asking for the value.
   /// If 'required' is true, an error will be given if the value is not defined.
   bool NumberGet(float* destination, const char* key, float defaultValue = 0.0f,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Double-precision version of NumberGet
   bool NumberGet(double* destination, const char* key, double defaultValue = 0.0,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assuming this is an object, load the boolean at the given key.
   /// If the item is not a boolean, set 'destination' to the 'defaultValue' and return false.
   /// Return true if successful.  'requesterType' indicates what type of object was asking for the value.
   /// 'requesterName' indicates the name of the instance of the object that was asking for the value.
   /// If 'required' is true, an error will be given if the value is not defined.
   bool BooleanGet(bool* destination, const char* key, bool defaultValue = false,
      const char* requesterType = NULL, const char* requesterName = NULL, bool required = false);

   /// Assemble a merged copy of JSON specifications which are defined
   /// recursively.  'this' is the JSON object containing the possible objects
   /// to be merged.  'name' gives the name of the next object in 'this' to
   /// process.  For the root call, this should be the most-derived name of the
   /// object.  An object can specify its parent or parents with the key
   /// specified in "parentKey".  For example, if parentKey is "Inherited", and
   /// the object's value for "Inherited" is a string, that string is the name
   /// of the the parent.  If "Inherited" is an array of strings, the named
   /// objects will be recursively processed and merged.  Parents listed
   /// earlier in the list will have lower priority when merging.
   /// 'workingSpecs' is for internal use.  Aliases can be used by defining a
   /// name in 'this' as a string rather than a JSON object.
   ///
   /// In the following example, Rex is a primarily a Juggler and secondarily a
   /// Dog (another word for Canine, which is a type of Animal) second.
   ///
   /// "Things":
   /// {
   ///    "Animal":
   ///    {
   ///       "EatsAndSleeps": true,
   ///       "Juggles": false,
   ///       "Unusual": false
   ///    },
   ///    "Canine":
   ///    {
   ///       "Inherited": "Animal",
   ///       "Barks": true
   ///    },
   ///    "Dog": "Canine",
   ///    "Juggler":
   ///    {
   ///       "Juggles": true
   ///    },
   ///    "Rex":
   ///    {
   ///       "Inherited": ["Dog", "Juggler"],
   ///       "Unusual": true
   ///    }
   /// }
   ///
   /// thingsObject->BuildRecursive("Rex") would return...
   /// {
   ///    "Inherited": ["Dog", "Juggler"],
   ///    "EatsAndSleeps": true,
   ///    "Barks": true,
   ///    "Juggles": true,
   ///    "Unusual": true
   /// }
   JSONValue* BuildRecursive(const char* name, HeapID heapID = HEAP_DEFAULT,
      HeapID tempHeapID = HEAP_TEMP, Table<const char*>* arraysToConcatenate = NULL,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);
   /// Rather than looking in 'this' for the object specified by 'name', use
   /// 'currentLayer' as the most derived version.
   JSONValue* BuildRecursive(JSONValue* currentLayer, HeapID heapID = HEAP_DEFAULT,
      HeapID tempHeapID = HEAP_TEMP, Table<const char*>* arraysToConcatenate = NULL,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);
   /// Similar to the simple form of BuildRecursive, except if an entry can't
   /// be found with the given name in 'this', it will look in the object
   /// 'fallbackObjects'.
   JSONValue* BuildRecursive(JSONValue* fallbackObjects, const char* name,
      HeapID heapID = HEAP_DEFAULT, HeapID tempHeapID = HEAP_TEMP,
      Table<const char*>* arraysToConcatenate = NULL,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);
   /// Rather than looking in 'this' or 'fallbackObjects' for the object
   /// specified by 'name', use 'currentLayer' as the most derived version.
   JSONValue* BuildRecursive(JSONValue* fallbackObjects, JSONValue* currentLayer,
      HeapID heapID = HEAP_DEFAULT, HeapID tempHeapID = HEAP_TEMP,
      Table<const char*>* arraysToConcatenate = NULL,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);
   /// Similar to the simple form of BuildRecursive, except instead of looking
   /// in 'this', it checks a sequence of JSON objects listed in 'objectGroups'.
   static JSONValue* BuildRecursive(Table<JSONValue*>* objectGroups, const char* name,
      HeapID heapID = HEAP_DEFAULT, HeapID tempHeapID = HEAP_TEMP,
      Table<const char*>* arraysToConcatenate = NULL,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);
   /// Rather than looking in 'objectGroups' for the object specified by
   /// 'name', use 'currentLayer' as the most derived version.
   static JSONValue* BuildRecursive(Table<JSONValue*>* objectGroups, JSONValue* currentLayer,
      HeapID heapID = HEAP_DEFAULT, HeapID tempHeapID = HEAP_TEMP,
      Table<const char*>* arraysToConcatenate = NULL,
      const char* parentKey = JSON_PARENT_KEY_DEFAULT, JSONValue* workingSpecs = NULL);

   /// Return the heap used for all allocations by this instance.
   HeapID HeapIDGet() { return heapID; }
   
protected:
   /// Assuming this is a JSON array, return a pointer to the item at the given
   /// index, or NULL if the index is invalid.
   JSONValue* ElementGet(int index);
   /// Assuming this is a JSON object, return a pointer to the item at the given
   /// key, or NULL if there is nothing at the given key (not even a JSONValue
   /// of TYPE_NULL).
   JSONValue* ElementGet(const char* key);
   
   /// Assuming this is a JSON array, return a pointer to the value at the given
   /// index if there is already a value at that index.  If the given index is
   /// greater than the current maximum index, new JSONValues of TYPE_NULL will
   /// be created as needed, and the value at the requested index will be returned.
   JSONValue* SetHelper(int index);
   /// Assuming this is a JSON object, return a pointer to the value with the
   /// given key if there is already a value for that key.  If not, create and
   /// return a new JSONValue of TYPE_NULL at the given key.  
   JSONValue* SetHelper(const char* key);
   
   /// The basic type of JSONValue this instance represents.
   /// We include the underscore to compensate for what seems to be a limitation of the
   /// custom visualization system in the Visual Studio 2005 debugger (autoexp.dat).
   ValueType _valueType;
   /// The underlying value for this instance.
   union
   {
      bool booleanValue;
      double numberValue;
      const char* stringValue;
      Map<const char*, JSONValue*>* objectValue;
      Table<JSONValue*>* arrayValue;    
   };
   /// Heap used for all allocations by this instance.
   HeapID heapID;
   /// Returned by the Get methods if an entry isn't defined.
   static JSONValue dummyValue;

   friend class JSONParser;
};

//==============================================================================

} //namespace Webfoot {

#endif //#ifndef __FROG__JSONVALUE_H__
